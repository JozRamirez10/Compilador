#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

// [0-9]+
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
// [a-zA-Z][a-zA-Z0-0]*
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

bool alphaNum(char** cadena){
    return id(cadena) || numero(cadena);
}

int saltarEspaciosVacios(char **cadena){
    while(**cadena == ' '){
        (*cadena)++;
    }
    return 0;
}

bool expresion(char** cadena){
    if(alphaNum(cadena)){
        saltarEspaciosVacios(cadena);
        if(**cadena == '+' || ** cadena == '-'){
            (*cadena)++;
            saltarEspaciosVacios(cadena);
            if(**cadena == '('){
                (*cadena)++;
                saltarEspaciosVacios(cadena);
                if(expresion(cadena)){
                    saltarEspaciosVacios(cadena);
                    if(**cadena == ')'){
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

// x = 1; || x = a;
bool asignacion(char** cadena){
    
    saltarEspaciosVacios(cadena);

    if(id(cadena)){
        saltarEspaciosVacios(cadena);
            if(**cadena == '='){
                (*cadena)++;
                saltarEspaciosVacios(cadena);
                if(expresion(cadena) || alphaNum(cadena)){
                    saltarEspaciosVacios(cadena);
                    if(**cadena == ';')
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

// Función para eliminar los "\n" de una cadena
void eliminarSalto(char *cadena) {
    char *posicionNuevaLinea = strchr(cadena, '\n');
    if (posicionNuevaLinea != NULL) {
        *posicionNuevaLinea = ' '; // Reemplaza el "\n" con el carácter nulo '\0'
    }
}

bool separarCadenas(char** cadena) {
    char *lineas[100];  // Declaración de un arreglo de apuntadores para las líneas
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
            eliminarSalto(copia);
            lineas[contador] = copia;
        }
    }

    // Si hay contenido después del último ";", guárdalo como una línea adicional
    if (*inicio != '\0') {
        eliminarSalto(inicio);
        lineas[contador] = strdup(inicio);
        contador++;
    }

    bool gramaticaValida = true;

    // Ahora puedes acceder a cada línea individualmente a través de lineas[i]
    for (int i = 0; i < contador; i++) {
        char* lineaTemp = lineas[i];
        if(!asignacion(&lineas[i])){
            printf("Error de gramática en la línea:\n");
            printf("  Linea %d: %s\n", i+1, lineaTemp);
            gramaticaValida = false;
        }
    }
    free(copia);  // Liberar la memoria de la copia
    
    if(gramaticaValida){
        printf("Gramática válida\n");
        return true;
    }

    return false;
}

int revisarGramatica(char* cadena){
    if(separarCadenas(&cadena)){
        return 0;
    }
    return 1;
}