/*
    Revisa que la gramática corresponda con lo planteado
    asignación => alphanum | expresion
    alphanum => id | numero
    id => [a-zA-Z][a-zA-Z0-9]*
    numero => [0-9]+
    expresion => alphanum + expresion | alphanum - expresion | expresion
*/
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

/*
    Revisa si el caracter es un numero
    Itera hasta no encontrar ningun digito
*/
bool numero(char** cadena){
    if(isdigit(**cadena)){
        (*cadena)++;
        while(isdigit(**cadena)){
            (*cadena)++;
        }
        return true;
    }
    return false;
}

/*
    Revisa que sea un caractér del alfabeto
    Despues del primer caracter es capaz de considerar números
    Hasta encontrar un caracter que no coincida
*/
bool id(char** cadena){
    if(isalpha(**cadena)){
        (*cadena)++;
        while(isalnum(**cadena)){
            (*cadena)++;
        }
        return true;
    }
    return false;
}

/*
    Revisa que el caracter corresponda al alfabeto
    o a un número
*/
bool alphaNum(char** cadena){
    return id(cadena) || numero(cadena);
}
/*
    Itera sobre los espacios vacios
*/
int saltarEspaciosVacios(char **cadena){
    while(**cadena == ' '){
        (*cadena)++;
    }
    return 0;
}

/*
    Revisa que se cumpla la gramática establecida para 
    la expresion
    Si no es una expresion retorna false
*/
bool expresion(char** cadena){
    if(alphaNum(cadena)){ // Valida si es alphaNum
        saltarEspaciosVacios(cadena);
        if(**cadena == '+' || ** cadena == '-'){ // Operadores: '+' o '-'
            (*cadena)++;
            saltarEspaciosVacios(cadena);
            if(**cadena == '('){ // Parentesis '('
                (*cadena)++;
                saltarEspaciosVacios(cadena);
                if(expresion(cadena)){ // Valida una expresion dentro del parentesis
                    saltarEspaciosVacios(cadena);
                    if(**cadena == ')'){ // Cierra parentesis ')'
                        (*cadena)++;
                        return true;
                    }
                    return false;
                }
            }else{
                if(expresion(cadena) || alphaNum(cadena))
                    return true;
            }
        }
        return true;
    }
    return false;
}

/*
    Valida que la asignación cumpla con la siguiente estructura:
    x = a;
    Si no cumple con la estructura retorna false
*/
bool asignacion(char** cadena){
    saltarEspaciosVacios(cadena);
    if(id(cadena)){ // Valida que el primer termino sea un id
        saltarEspaciosVacios(cadena);
            if(**cadena == '='){ // Operador de asignación
                (*cadena)++;
                saltarEspaciosVacios(cadena);
                if(expresion(cadena) || alphaNum(cadena)){ // Expresion | Alphanum
                    saltarEspaciosVacios(cadena);
                    if(**cadena == ';') // Debe terminar con ';'
                        return true;
                    return false;
                }
                if(cadena != NULL && **cadena == '\0'){
                    return false;
                }
            }
            if(cadena != NULL && **cadena == '\0'){
                return false;
            }
    }
    return false;
}

/*
    Función para eliminar los "\n" de una cadena
    Los remplaza por espacios
*/
void eliminarSalto(char *cadena) {
    char *posicionNuevaLinea = strchr(cadena, '\n'); // Busca si dentro del apuntador hay un '\n'
    if (posicionNuevaLinea != NULL) {
        *posicionNuevaLinea = ' '; // Reemplaza el "\n" con ' '
    }
}

/*
    Separa el contenido por líneas para poder verificar la gramática en cada una
    Si la gramatica no se cumple en alguna línea retorna false
*/
bool separarCadenas(char** cadena) {
    char *lineas[100];  // Almacena cada una de las líneas
    char* copia = strdup(*cadena);  // Hacer una copia de la cadena original
    char* inicio = copia; // Puntero para rastrear el inicio de la línea
    int contador = 0;

    // Recorrer la cadena copiada en busca de ";"
    for (int i = 0; copia[i] != '\0'; i++) {
        if (copia[i] == ';') {
            // Encontramos un punto y coma, copiamos la subcadena y la almacenamos en lineas
            char* linea = (char*)malloc(i - (inicio - copia) + 2); // +2 para ";" y el carácter nulo '\0'
            strncpy(linea, inicio, i - (inicio - copia) + 1);
            linea[i - (inicio - copia) + 1] = '\0';
            eliminarSalto(linea);
            lineas[contador] = linea; // Almacena la línea en el arreglo de apuntadores
            contador++;
            inicio = &copia[i + 1]; // Actualiza el inicio para la próxima línea
        }else{
            eliminarSalto(copia); // Si solo tiene una sola línea sin ';'
            lineas[contador] = copia;
        }
    }

    // Si hay contenido después del último ";", guárdalo como una línea adicional
    if (*inicio != '\0') {
        eliminarSalto(inicio);
        lineas[contador] = strdup(inicio);
        contador++;
    }

    bool gramaticaValida = true; // Boolean que valida la gramática

    // Itera sobre cada línea 
    for (int i = 0; i < contador; i++) {
        char* lineaTemp = lineas[i];
        if(!asignacion(&lineas[i])){ // Verifica que se cumpla la gramática para cada línea
            printf("Error de gramática en la línea:\n");
            printf("  Linea %d: %s\n", i+1, lineaTemp);
            gramaticaValida = false;
        }
    }
    free(copia);  // Liberar la memoria de la copia
    
    if(gramaticaValida) // Si la gramatica es válida para todas las líneas devuelve true
        return true;

    return false;
}
/*
    Llama a las demás funciones para verificar que se cumpla la gramática
*/
int revisarGramatica(char* cadena){
    if(separarCadenas(&cadena)){
        return 0;
    }
    return 1; // Error en la gramática
}