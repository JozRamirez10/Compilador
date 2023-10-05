#include "leer.c"
#include "analizador.c"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    
    char* contenido;
    
    leer(argv, &contenido);
    analizador(contenido);
    
    free(contenido);
    return 0;
}