/*
    Funciones para la creación de código en lenguaje ensamblador
*/
#include <stdio.h>
#include <string.h>

/*
    Declaración de funciones
*/
int crearArchivo(char* nombre, char* contenido);
char* imprimeSalida();
void operacion(struct NodoAST* nodo);
void agregarOperacion(struct NodoAST* nodo);
void generaCodigo(struct NodoAST* nodo);

/*
    Plantilla del archivo de salida
*/
char sectionData[15] = "section .data\n";
char sectionText[30] = "section .text\n  global main\n";
char sectionMain[10] = "main:\n";
char sectionEnd[20] = "\n  mov eax, 0\n  ret\n";
char asignaciones[200];
char codigo[200];
char salida[1000];
int cerrarID = 0;

/*
    Crea el archivo "obj" y escribe el contenido en él
*/
int crearArchivo(char* nombre, char* contenido){
    strcat(nombre, ".obj");
    printf("%s\n",nombre);
    FILE *archivo = fopen(nombre, "w");
    if(archivo != NULL){
        fputs(contenido, archivo);
        fclose(archivo);
    }else{
        return -1;
    } 
    return 1;
}

/*
    Almacena el contenido en la variable de salida
*/
char* imprimeSalida(){
    strcat(salida, sectionData);
    strcat(salida, asignaciones);
    strcat(salida, sectionText);
    strcat(salida, sectionMain);
    strcat(salida, codigo);
    strcat(salida, sectionEnd);
    printf("%s", salida);
    return salida;
}

/*
    Valida la operación de forma iterativa
*/
void operacion(struct NodoAST* nodo){
    if(nodo != NULL){
        char aux[100];
        //char aux2[50];
        if(nodo->tipo == SUMA_NODO){
            sprintf(aux, "  add eax, %s\n", nodo->derecho->izquierdo->lexema);
            strcat(codigo, aux);
            if(nodo->derecho->derecho->tipo == INT_NODO && nodo->derecho->tipo == SUMA_NODO){
                sprintf(aux, "  add eax, %s\n", nodo->derecho->derecho->lexema);
                strcat(codigo, aux);
            }else if(nodo->derecho->derecho->tipo == INT_NODO && nodo->derecho->tipo == RESTA_NODO){
                sprintf(aux, "  sub eax, %s\n", nodo->derecho->derecho->lexema);
                strcat(codigo, aux);
            }else if(nodo->derecho->derecho->tipo == SUMA_NODO || nodo->derecho->derecho->tipo == RESTA_NODO)
                operacion(nodo->derecho);
        }else if(nodo->tipo == RESTA_NODO){
            sprintf(aux, "  sub eax, %s\n", nodo->derecho->izquierdo->lexema);
            strcat(codigo, aux);
            if(nodo->derecho->derecho->tipo == INT_NODO && nodo->derecho->tipo == SUMA_NODO){
                sprintf(aux, "  add eax, %s\n", nodo->derecho->derecho->lexema);
                strcat(codigo, aux);
            }else if(nodo->derecho->derecho->tipo == INT_NODO && nodo->derecho->tipo == RESTA_NODO){
                sprintf(aux, "  sub eax, %s\n", nodo->derecho->derecho->lexema);
                strcat(codigo, aux);
            }else if(nodo->derecho->derecho->tipo == SUMA_NODO || nodo->derecho->derecho->tipo == RESTA_NODO)
                operacion(nodo->derecho);
        }
    }
}

/*
    Valida la operación
*/
void agregarOperacion(struct NodoAST* nodo){
    if(nodo != NULL){
        char aux[50];
        char aux2[50];
        if(nodo->tipo == SUMA_NODO){
            if(nodo->izquierdo->tipo == INT_NODO){
                sprintf(aux, "  mov eax, %s\n", nodo->izquierdo->lexema);
                strcat(codigo, aux);
            }else if(nodo->izquierdo->tipo == ID_NODO){
                sprintf(aux, "  mov eax, dword[%s]\n", nodo->izquierdo->lexema);
                strcat(codigo, aux);
                cerrarID = 1;
            }
            if(nodo->derecho->tipo == INT_NODO){
                sprintf(aux2, "  add eax, %s\n", nodo->derecho->lexema);
                strcat(codigo, aux2);
            }else if(nodo->derecho->tipo == SUMA_NODO ||nodo->derecho->tipo == RESTA_NODO){
                operacion(nodo);
            }
        }else if(nodo->tipo == RESTA_NODO){
            if(nodo->izquierdo->tipo == INT_NODO){
                sprintf(aux, "  mov eax, %s\n", nodo->izquierdo->lexema);
                strcat(codigo, aux);
            }
            if(nodo->derecho->tipo == INT_NODO){
                sprintf(aux2, "  sub eax, %s\n", nodo->derecho->lexema);
                strcat(codigo, aux2);
            }else if(nodo->derecho->tipo == SUMA_NODO ||nodo->derecho->tipo == RESTA_NODO)
                operacion(nodo);
        }
    }
}

/*
    Revisa el caso de cada variable declarada y genera el código
*/
void generaCodigo(struct NodoAST* nodo){
    if(nodo != NULL){
        if(nodo->tipo == ASIGNACION_NODO){
            if(nodo->izquierdo->tipo == ID_PRINCIPAL){
                char aux[40];
                char aux2[40];
                switch(nodo->derecho->tipo){
                    case INT_NODO:
                        sprintf(aux, "  %s dd 0\n", nodo->izquierdo->lexema);
                        sprintf(aux2, "  mov dword [%s], %s\n", nodo->izquierdo->lexema, nodo->derecho->lexema);
                        strcat(codigo, aux2);
                        break;
                    case CHAR_NODO:
                        sprintf(aux, "  %s db 0\n", nodo->izquierdo->lexema);
                        sprintf(aux2, "  mov byte [%s], %s\n", nodo->izquierdo->lexema, nodo->derecho->lexema);
                        strcat(codigo, aux2);
                        break;
                    case ID_NODO:
                        sprintf(aux, "  %s dd 0\n", nodo->izquierdo->lexema);
                        sprintf(aux2, "  mov eax, dword [%s]\n", nodo->derecho->lexema);
                        strcat(codigo, aux2);
                        sprintf(aux2, "  mov dword [%s], eax\n", nodo->izquierdo->lexema);
                        strcat(codigo, aux2);
                        break;
                    case SUMA_NODO:
                        sprintf(aux, "  %s dd 0\n", nodo->izquierdo->lexema);
                        agregarOperacion(nodo->derecho);
                        break;
                    case RESTA_NODO:
                        sprintf(aux, "  %s dd 0\n", nodo->izquierdo->lexema);
                        agregarOperacion(nodo->derecho);
                        break;
                    default:
                        break;
                }
                strcat(asignaciones, aux);
                if(cerrarID){
                    sprintf(aux, "  mov dword [%s], eax\n", nodo->izquierdo->lexema);
                    strcat(codigo, aux);
                    cerrarID = 0;
                }
            }
        }
    }
}   