/*
    Función principal desde donde se ejecutan todos los procesos
*/

// Archivos con estructuras y funciones
#include "tokens.c"
#include "errores.c"
#include "nodoAst.c"
#include "tablaSimbolos.c"

// Archivos dedicados a funciones específicas
#include "leer.c"
#include "lexico.c"
#include "sintactico.c"
#include "semantico.c"
#include "generarCodigo.c"
#include "ast.c"

#include <stdio.h>
#include <stdlib.h>
/*
    Recibe como argumento el nombre del fichero que se quiere analizar
*/
int main(int argc, char* argv[]){

    char* contenido; // Esta variable va almacenar el contenido del fichero
    struct ArrayError errores; // Almacena los errores
    errores.numErrores = 0;
    struct Token* tokens; // Almacena los tokens
    int contadorTokens;

    struct Pila pila;
    char* output; // Contenido de la generación de código

    leer(argv, &contenido); // Se encarga de leer el fichero y guardar su contenido 

    // Obtiene el total de tokens que hay en la cadena
    contadorTokens = lexico(contenido, &tokens, &errores); // Convierte los lexemas a tokens
    sintactico(tokens, contadorTokens, &errores); // Verifica que se cumpla la gramática con respecto a los tokens
    
    if(errores.numErrores > 0){    // Errores en el análisis semantico
        imprimirErrores(&errores, tokens, contadorTokens);
    }else{
        inicializarPila(&pila);
        construccionAST(tokens, contadorTokens, &pila, &errores); // Crea el AST, la tabla de simbolos y genera código
        if(errores.numErrores > 0){
            imprimirErrores(&errores, tokens, contadorTokens);
        }else{
            if(!(crearArchivo(argv[1], imprimeSalida()))) // Crea el "obj"
                printf("Error al crear el archivo 'obj'");
        }
        // Vacia la pila
        while (pila.tope != NULL) {
        pop(&pila);
        }
    } 
    // Libera el espacio de los apuntadores
    free(tokens);
    free(contenido);
    
    return 0;
}