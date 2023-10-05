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
        int num = 1;
        char* numLinea;
        while (fgets(linea, sizeof(linea), f) != NULL) {
            //asprintf(&numLinea, "%d", num);
            //strcat(*contenido, numLinea);
            // strcat(*contenido, " - ");
            strcat(*contenido, linea);
            //strcat(*contenido, "\n");
            num++;
        }
        fclose(f);
    }
    return 0;
}
