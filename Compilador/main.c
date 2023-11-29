/*
    Función principal desde donde se ejecuta el analizador léxico
*/

// Archivos con estructuras y funciones
#include "tokens.c"
#include "ast.c"

// Archivos dedicados a funciones específicas
#include "leer.c"
#include "lexico.c"
#include "sintactico.c"

#include <stdio.h>
#include <stdlib.h>
/*
    Recibe como argumento el nombre del fichero que se quiere analizar
*/
int main(int argc, char* argv[]){

    char* contenido; // Esta variable va almacenar el contenido del fichero
    struct ArrayError errores; // Almacena los errores
    errores.numErrores = 0;
    struct Token* tokens;
    int contadorTokens;

    leer(argv, &contenido); // Se encarga de leer el fichero y guardar su contenido 

    // Obtiene el total de tokens que hay en la cadena
    contadorTokens = lexico(contenido, &tokens, &errores); // Convierte los lexemas a tokens
    sintactico(tokens, contadorTokens, &errores); // Verifica que se cumpla la gramática con respecto a los tokens
    
    if(errores.numErrores > 0){    
        imprimirErrores(&errores, tokens, contadorTokens);
    }else{
        printf("Valid\n"); // Se tienen que cumplir todas las producciones
        ast(tokens, contadorTokens);
    }
    
    free(tokens);
    free(contenido); 
    
    return 0;
}