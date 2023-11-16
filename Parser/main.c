/*
    Función principal desde donde se ejecuta el analizador léxico
*/

// Archivos dedicados a funciones específicas
#include "tokens.c"
#include "leer.c"
#include "gramatica.c"
#include "analizador.c"
#include "parser.c"
#include "ast.c"

#include <stdio.h>
#include <stdlib.h>
/*
    Recibe como argumento el nombre del fichero que se quiere analizar
*/
int main(int argc, char* argv[]){
    
    char* contenido; // Esta variable va almacenar el contenido del fichero
    struct Token* tokens;
    int contadorTokens;

    leer(argv, &contenido); // Se encarga de leer el fichero y guardar su contenido 
    
    // Obtiene el total de tokens que hay en la cadena
    contadorTokens = analizador(contenido, &tokens); // Convierte los lexemas a tokens
    if(parser(tokens, contadorTokens)){ // Verifica que se cumpla la gramática con respecto a los tokens
        printf("Valid\n"); // Se tienen que cumplir todas las producciones
        ast(tokens, contadorTokens);
    }else{ // Si no se cumple alguna regla de producción lanza un error
        printf("Error\n");
    }
    
    free(tokens);
    free(contenido);
    
    return 0;
}