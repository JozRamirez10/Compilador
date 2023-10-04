#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int leer(char* argv[], char** contenido){
    FILE *f;
    f = fopen(argv[1], "r");
    if(f == NULL){
        printf("Error");
        exit(1);
    }else{
        fseek(f, 0, SEEK_END);
        int size = ftell(f);

        *contenido = (char*)malloc(sizeof(char) * (size + 1));
        rewind(f);

        (*contenido)[0] = '\0';

        char linea[256];  // Supongamos que ninguna línea tiene más de 255 caracteres
        while (fgets(linea, sizeof(linea), f) != NULL) {
            strcat(*contenido, linea);
            strcat(*contenido, "\n");
        }
        fclose(f);
    }
    return 0;
}
