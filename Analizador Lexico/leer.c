/*
    Esta función se encarga de leer el fichero y guardar su contenido
    en un apuntador de tipo char
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    argv - Nombre del fichero
    contenido - Apuntador donde se almacena el contenido del fichero
*/
int leer(char* argv[], char** contenido){
    
    FILE *f;
    f = fopen(argv[1], "r"); // Abre el archivo en modo lectura 
    
    if(f == NULL){ // Busca errores en el archivo
        printf("Error");
        exit(1);
    
    }else{
        fseek(f, 0, SEEK_END); // Mueve el puntero del archivo al final 
        int size = ftell(f); // Obtiene el tamaño de carácteres

        *contenido = (char*)malloc(sizeof(char) * (size + 1));
        rewind(f); // Vuelve a colocar el puntero del archivo al inicio

        char linea[256]; // Consideramos que el tamaño de la linea es de 256 caracteres
        while (fgets(linea, sizeof(linea), f) != NULL) { // Obtiene línea por línea del fichero
            strcat(*contenido, linea); // Copia la línea dentro del apuntador
        }
        fclose(f);
    }
    return 0;
}
