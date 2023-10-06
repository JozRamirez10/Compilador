/*
    Función principal desde donde se ejecuta el analizador léxico
*/

// Archivos dedicados a funciones específicas
#include "leer.c"
#include "gramatica.c"
#include "analizador.c"

#include <stdio.h>
#include <stdlib.h>
/*
    Recibe como argumento el nombre del fichero que se quiere analizar
*/
int main(int argc, char* argv[]){
    
    char* contenido; // Esta variable va almacenar el contenido del fichero
    
    leer(argv, &contenido); // Se encarga de leer el fichero y guardar su contenido 
    if(revisarGramatica(contenido) == 0){ // Revisa que la grámatica sea correcta
        analizador(contenido); // Convierte los lexemas a tokens
    }
    free(contenido);
    return 0;
}