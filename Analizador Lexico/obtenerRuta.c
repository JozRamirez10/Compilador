#include <stdio.h>

int imprimirRuta(int argc, char* argv[]){
    for(int i = 1; i < argc; i++){
        printf("%s\n", argv[i]); 
    }
    printf("Nombre del archivo\n");
    return 0;
}