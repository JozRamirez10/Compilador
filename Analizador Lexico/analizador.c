#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Definición de tokens
enum {
    TOKEN_IDENTIFICADOR,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_ENTERO,
    TOKEN_FLOTANTE,
    TOKEN_ASIGNACION,
    TOKEN_SUMA,
    TOKEN_RESTA,
    TOKEN_MULTIPLICACION,
    TOKEN_DIVISION,
    TOKEN_PARENTESIS_IZQ,
    TOKEN_PARENTESIS_DER,
    TOKEN_LLAVE_ABRE,
    TOKEN_LLAVE_CIERRA,
    TOKEN_COMA,
    TOKEN_PUNTO_Y_COMA,
    TOKEN_COMENTARIO,
    TOKEN_ERROR,
    TOKEN_EOF, 
    TOKEN_SALTO,
};

// Estructura para almacenar información sobre los tokens
struct Token {
    int tipo;
    char lexema[100];
};

// Función para obtener el siguiente token desde la entrada
struct Token obtenerSiguienteToken(char** ptr) {
    struct Token token;

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
            token.tipo = TOKEN_IF;
        } else if (strcmp(token.lexema, "else") == 0) {
            token.tipo = TOKEN_ELSE;
        } else if (strcmp(token.lexema, "while") == 0) {
            token.tipo = TOKEN_WHILE;
        } else {
            token.tipo = TOKEN_IDENTIFICADOR;
        }
        (*ptr)--;   
    }else if (isdigit(**ptr) || **ptr == '.') {
        // Reconocer números enteros y números de punto flotante
        int i = 0;
        int puntoDecimal = 0;
        int error = 0;

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
            token.tipo = TOKEN_ERROR;
        } else{
            if (puntoDecimal == 0) {
                token.tipo = TOKEN_ENTERO;
            } else {
                token.tipo = TOKEN_FLOTANTE;
            }
        }
        (*ptr)--;
    }else {
        // Reconocer operadores y otros símbolos
        switch (**ptr) {
            case '=':
                token.tipo = TOKEN_ASIGNACION;
                break;
            case '+':
                token.tipo = TOKEN_SUMA;
                break;
            case '-':
                token.tipo = TOKEN_RESTA;
                break;
            case '*':
                token.tipo = TOKEN_MULTIPLICACION;
                break;
            case '/':
                (*ptr)++;
                if(**ptr == '/'){
                    token.tipo = TOKEN_COMENTARIO;
                    while(**ptr != '\n'){
                        (*ptr)++;
                    }
                }else{
                    token.tipo = TOKEN_DIVISION;
                    (*ptr)--;
                }
                break;
            case '(':
                token.tipo = TOKEN_PARENTESIS_IZQ;
                break;
            case ')':
                token.tipo = TOKEN_PARENTESIS_DER;
                break;
            case '{':
                token.tipo = TOKEN_LLAVE_ABRE;
                break;
            case '}':
                token.tipo = TOKEN_LLAVE_CIERRA;
                break;
            case ',':
                token.tipo = TOKEN_COMA;
                break;
            case ';':
                token.tipo = TOKEN_PUNTO_Y_COMA;
                break;
            case '\n':
                token.tipo = TOKEN_SALTO;
                break;
            default:
                token.tipo = TOKEN_ERROR; // Carácter no reconocido
                break;
        }
        token.lexema[0] = **ptr;
        token.lexema[1] = '\0';
    }
    return token;
}

int analizador(char* contenido) {
    char *ptr = contenido;
    struct Token token;
    char salida[500];
    strcpy(salida, "");

    printf("\nAnalizador léxico:******************** \n");
    
    do{
        token = obtenerSiguienteToken(&ptr);
        switch (token.tipo) {
            case TOKEN_IDENTIFICADOR:
                printf("\tTOKEN_IDENTIFICADOR: %s\n", token.lexema);
                strcat(salida, "IDENTIFICADOR");
                break;
            case TOKEN_IF:
                printf("\tTOKEN_IDENTIFICADOR: %s\n", token.lexema);
                strcat(salida, "IF");
                break;
            case TOKEN_ELSE:
                printf("\tTOKEN_IDENTIFICADOR: %s\n", token.lexema);
                strcat(salida, "ELSE");
                break;
            case TOKEN_WHILE:
                printf("\tTOKEN_IDENTIFICADOR: %s\n", token.lexema);
                strcat(salida, "WHILE");
                break;
            case TOKEN_ENTERO:
                printf("\tTOKEN_ENTERO: %s\n", token.lexema);
                strcat(salida, "ENTERO");
                break;
            case TOKEN_FLOTANTE:
                printf("\tTOKEN_FLOTANTE: %s\n", token.lexema);
                strcat(salida, "FLOTANTE");
                break;
            case TOKEN_ASIGNACION:
                printf("\tTOKEN_ASIGNACION: %s\n", token.lexema);
                strcat(salida, "ASIGNACION");
                break;
            case TOKEN_SUMA:
                printf("\tTOKEN_SUMA: %s\n", token.lexema);
                strcat(salida, "SUMA");
                break;
            case TOKEN_RESTA:
                printf("\tTOKEN_RESTA: %s\n", token.lexema);
                strcat(salida, "RESTA");
                break;
            case TOKEN_MULTIPLICACION:
                printf("\tTOKEN_MULTIPLICACION: %s\n", token.lexema);
                strcat(salida, "MULT");
                break;
            case TOKEN_DIVISION:
                printf("\tTOKEN_DIVISION: %s\n", token.lexema);
                strcat(salida, "DIV");
                break;
            case TOKEN_PARENTESIS_IZQ:
                printf("\tTOKEN_PARENTESIS_IZQ: %s\n", token.lexema);
                strcat(salida, "PARENT_IZQ");
                break;
            case TOKEN_PARENTESIS_DER:
                printf("\tTOKEN_PARENTESIS_DER: %s\n", token.lexema);
                strcat(salida, "PARENT_DER");
                break;
            case TOKEN_LLAVE_ABRE:
                printf("\tTOKEN_LLAVE_ABRE: %s\n", token.lexema);
                strcat(salida, "LLAVE_ABRE");
                break;
            case TOKEN_LLAVE_CIERRA:
                printf("\tTOKEN_LLAVE_CIERRA: %s\n", token.lexema);
                strcat(salida, "LLAVE_CIERRA");
                break;
            case TOKEN_COMA:
                printf("\tTOKEN_COMA: %s\n", token.lexema);
                strcat(salida, "COMA");
                break;
            case TOKEN_PUNTO_Y_COMA:
                printf("\tTOKEN_PUNTO_Y_COMA: %s\n", token.lexema);
                strcat(salida, "PUNTO_Y_COMA");
                break;
            case TOKEN_COMENTARIO:
                printf("\tTOKEN_COMENTARIO: //\n");
                break;
            case TOKEN_SALTO:
                printf("\n");
                break;
            case TOKEN_ERROR:
                printf("\tTOKEN_ERROR: %s\n", token.lexema);
                break;
        }
        if(token.tipo == TOKEN_SALTO){
            strcat(salida, "\n");
        }else{
            if(token.tipo != TOKEN_ERROR){
                strcat(salida, " ");
            }
        }
        ptr++;
    }while(*ptr != '\0');
    
    printf("*************************** \n");
    
    printf("\nEntrada:******************** \n\n");
    printf("%s\n\n", contenido);
    printf("*************************** \n");

    printf("\nSalida:******************** \n\n");
    printf("%s\n\n", salida);
    printf("*************************** \n\n");
    
    return 0;
}
