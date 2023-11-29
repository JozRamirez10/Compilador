// Errores léxicos -> Identifiación de tokens
// Errores sintácticos -> Los tokens no cumplen con la gramática

#include <stdio.h>

typedef enum{
    LEXICO, // Token no reconocido
    SINTACTICO, // No cumple con la gramática
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

void imprimirErrores(struct ArrayError* errores, struct Token* tokens, int contadorTokens){
    
    for(int i = 0; i < errores->numErrores; i++){
        switch (errores->error[i].tipo)
        {
        case LEXICO:
            printf("Linea %d | Token sin reconocer (Error léxico): %s\n", errores->error[i].linea ,errores->error[i].lexema);
            break;
        case SINTACTICO:
            printf("Linea %d | No cumple con la gramatica (Error sintactico): ", errores->error[i].linea);
            int j = 0;
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