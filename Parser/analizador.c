/*
    Obtiene los tokens que le corresponden a los lexemas
*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/*
    Itera sobre cada caracter para encontrar el tipo de token
*/
struct Token obtenerSiguienteToken(char** ptr) {
    struct Token token;
#
    // Ignorar espacios en blanco y tabulaciones
    while (**ptr  == ' ' || **ptr == '\t'){
        (*ptr)++;
    }

    if (isalpha(**ptr)) { // Verifica si es una letra (mayúscula o minúscula)
        
        // Reconocer identificadores o palabras clave
        int i = 0;
        do {
            token.lexema[i++] = **ptr;
            (*ptr)++;
        } while (isalnum(**ptr) || **ptr == '_'); // Verifica si es letra, número o _
        token.lexema[i] = '\0';
        
        if (strcmp(token.lexema, "if") == 0) {
            token.tipo = IF;
        } else if (strcmp(token.lexema, "else") == 0) {
            token.tipo = ELSE;
        } else if (strcmp(token.lexema, "while") == 0) {
            token.tipo = WHILE;
        } else {
            token.tipo = ID;
        }
        (*ptr)--; 

    }else if (isdigit(**ptr) || **ptr == '.') { // Valida que sea digito o "."
        
        // Reconocer números enteros y números de punto flotante
        int i = 0;
        int puntoDecimal = 0;
        int error = 0; // Error dentro de los digitos, Error: 1a

        do {
            if (**ptr == '.') {
                puntoDecimal++;
            }
            token.lexema[i++] = **ptr;
            (*ptr)++;
            
            if (isalpha(**ptr)){
                token.lexema[i++] = **ptr;
                error++;
            }

        } while (isdigit(**ptr) || (**ptr == '.' && puntoDecimal == 0));
        
        token.lexema[i] = '\0';
        if (error != 0){
            token.tipo = ERROR;
        } else{
            if (puntoDecimal == 0) {
                token.tipo = INT;
            } else {
                token.tipo = FLOAT;
            }
        }
        (*ptr)--;
    }else {
        // Reconocer operadores y otros símbolos
        switch (**ptr) {
            case '=':
                token.tipo = ASIGNACION;
                break;
            case '+':
                token.tipo = SUMA;
                break;
            case '-':
                token.tipo = RESTA;
                break;
            case '*':
                token.tipo = MULT;
                break;
            case '/': // Verifica que se division o comentario
                (*ptr)++;
                if(**ptr == '/'){
                    token.tipo = COMENTARIO;
                    while(**ptr != '\n'){
                        (*ptr)++;
                    }
                }else{
                    token.tipo = DIV;
                    (*ptr)--;
                }
                break;
            case '(':
                token.tipo = PARENT_IZQ;
                break;
            case ')':
                token.tipo = PARENT_DER;
                break;
            case '{':
                token.tipo = KEY_DER;
                break;
            case '}':
                token.tipo = KEY_IZQ;
                break;
            case ',':
                token.tipo = COMA;
                break;
            case ';':
                token.tipo = FIN;
                break;
            case '\n':
                token.tipo = SALTO;
                break;
            default:
                token.tipo = ERROR; // Carácter no reconocido
                break;
        }
        token.lexema[0] = **ptr; // Guarda el lexema
        token.lexema[1] = '\0';
    }
    return token;
}

/*
    Incrementa el valor del apuntador de arreglo de tokens
*/
void verificarTamañoSalida(int* capacidad, int* contadorTokens, struct Token** tokensSalida){
    if(*contadorTokens >= *capacidad){ // Si es contador ya supero la capacidad
        *capacidad *= 2; // Duplica la capacidad
        struct Token* temp = (struct Token*)realloc(*tokensSalida, (*capacidad) * sizeof(struct Token)); 
        *tokensSalida = temp; // Se asigna la nueva capacidad
    }
}

/*
    Devuelve la cantidad de tokens que ha encontrado
    Itera sobre el contenido para validar cada tipo de token
*/
int analizador(char* contenido, struct Token** tokensSalida) {
    char *ptr = contenido; // El apuntador itera sobre el contenido
    struct Token token; 
    
    int capacidad = 10;
    int contadorTokens = 0;
    *tokensSalida = (struct Token*)malloc(capacidad * sizeof(struct Token));

    char salida[500]; // Guarda el tipo de tokens a la salida 
    strcpy(salida, ""); // Inicializa el array

    
    do{
        token = obtenerSiguienteToken(&ptr); // Busca los tokens en el contenido del archivo
        if(token.tipo != 19 && token.tipo != 18){ // Omite los saltos de línea y espacios
            verificarTamañoSalida(&capacidad, &contadorTokens, tokensSalida); 
            // Asigna los valores del token al arreglo de tokens
            (*tokensSalida)[contadorTokens].tipo = token.tipo;
            strcpy((*tokensSalida)[contadorTokens].lexema, token.lexema);
            contadorTokens++;
        }
        ptr++;
    }while(*ptr != '\0');

    /* printf("*************************** \n");
    
    printf("\nEntrada:******************** \n\n");
    printf("%s\n\n", contenido);
    printf("*************************** \n");

    printf("\nSalida:******************** \n\n");
    printf("%s\n\n", salida);
    printf("*************************** \n\n"); */

    return contadorTokens;
}
