// Errores léxicos -> Identifiación de tokens
// Errores sintácticos -> Los tokens no cumplen con la gramática
// Errores semántico -> No tiene coherencia el código fuente
#include <stdio.h>

typedef enum{
    LEXICO, // Token no reconocido
    SINTACTICO, // No cumple con la gramática
    SEMANTICO_SNI, // Simbolo no identificado 
    SEMANTICO_TI, // Tipos incompatibles
} TipoError;

typedef struct Error{
    TipoError tipo;
    char lexema[20];
    int linea;
} Error;

typedef struct ArrayError{
    struct Error* error;
    int numErrores;
} ArrayError;

void imprimirErrores(struct ArrayError* errores, struct Token* tokens, int contadorTokens);

void imprimirErrores(struct ArrayError* errores, struct Token* tokens, int contadorTokens){
    
    for(int i = 0; i < errores->numErrores; i++){
        switch (errores->error[i].tipo)
        {
        case LEXICO:
            printf("Linea %d | (Error léxico) Token sin reconocer: %s\n", errores->error[i].linea ,errores->error[i].lexema);
            break;
        case SINTACTICO:
            printf("Linea %d | (Error sintáctico) No cumple con la gramatica: ", errores->error[i].linea);
            int j = 0;
            while(j < contadorTokens){
                if(tokens[j].linea == errores->error[i].linea){
                    printf("%s ", tokens[j].lexema);
                }
                j++;
            }
            printf("\n");
            break;
        case SEMANTICO_SNI:
            printf("Linea %d | (Error semántico) No se encontró el simbolo: %s\n", errores->error[i].linea ,errores->error[i].lexema);
            break;
        case SEMANTICO_TI:
            printf("Linea %d | (Error semántico) Los tipos son incompatibles: ", errores->error[i].linea);
            j = 0;
            while(j < contadorTokens){
                if(tokens[j].linea == errores->error[i].linea){
                    printf("%s ", tokens[j].lexema);
                }
                j++;
            }
            printf("\n");
            break;
        default:
            break;
        }
    }
}