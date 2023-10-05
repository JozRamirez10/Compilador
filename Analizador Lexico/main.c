#include "leer.c"
#include "gramatica.c"
#include "analizador.c"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    // imprimirRuta(argc, argv);
    
    char* contenido;
    leer(argv, &contenido);
    if(revisarGramatica(contenido) == 0){
        printf("Validación correcta");
        analizador(contenido);
        free(contenido);
    }
    return 0;
}