/*
    Funciones que realizan el análisis semántico
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
    Declaración de funciones
*/
void semantico1(NodoAST* nodo, int nivel, struct Pila* pila, struct ArrayError* errores);
void semantico2(NodoAST* nodo, int nivel, struct Pila* pila, struct ArrayError* errores);
int comprobarOperaciones(NodoAST* nodo, struct ArrayError* errores, struct Pila* pila);
int asignarValor(NodoAST* nodo, struct ArrayError* errores, struct Pila* pila);

/*
    Revisa la declaración de variables
*/
void semantico1(NodoAST* nodo, int nivel, struct Pila* pila, struct ArrayError* errores){
    if(nodo != NULL){
        struct Simbolo aux;
        int contadorErrores = errores->numErrores;
        if(nodo->tipo == ID_PRINCIPAL){
            strcpy(aux.lexema, nodo->lexema);
            push(pila, aux);
        }else{
            if(nodo->tipo == ID_NODO){
                aux = buscarSimbolo(pila, nodo->lexema);
                if(strcmp(aux.lexema, "NULL") == 0){
                    errores->numErrores = ++contadorErrores;
                    if(contadorErrores == 1){
                        errores->error = (struct Error*)malloc(sizeof(struct Error));
                    }else{
                        struct Error* temp = (struct Error*)realloc(errores->error, (contadorErrores)*sizeof(struct Error));
                    }
                    errores->error[contadorErrores-1].tipo = SEMANTICO_SNI;
                    strcpy(errores->error[contadorErrores-1].lexema, nodo->lexema);
                    errores->error[contadorErrores-1].linea = nodo->linea;
                }
            }
        }
        
        semantico1(nodo->izquierdo, nivel + 1, pila, errores);
        semantico1(nodo->derecho, nivel + 1, pila, errores);
    }
}


/*
    Comprueba que las operaciones se correspondas con los datos
*/
int comprobarOperaciones(NodoAST* nodo, struct ArrayError* errores, struct Pila* pila ){
    if(nodo != NULL){
        if(nodo->izquierdo->tipo == nodo->derecho->tipo){
            if(nodo->izquierdo->tipo != ID_NODO){
                return nodo->izquierdo->tipo;
            }else{
                Simbolo aux1 = buscarSimbolo(pila, nodo->izquierdo->lexema);
                Simbolo aux2 = buscarSimbolo(pila, nodo->derecho->lexema);
                if((strcmp(aux1.lexema, "NULL") != 0) && (strcmp(aux2.lexema, "NULL") != 0)){
                    if(aux1.tipo == aux2.tipo){
                        return aux1.tipo;
                    }
                }
            }
        }else if(nodo->izquierdo->tipo == ID_NODO){
            Simbolo aux = buscarSimbolo(pila, nodo->izquierdo->lexema);
            if(strcmp(aux.lexema, "NULL") != 0){
                if(aux.tipo == nodo->derecho->tipo){
                    return nodo->derecho->tipo;
                }
            }
        }else if(nodo->derecho->tipo == ID_NODO){
            Simbolo aux = buscarSimbolo(pila, nodo->derecho->lexema);
            if(strcmp(aux.lexema, "NULL") != 0){
                if(aux.tipo == nodo->izquierdo->tipo){
                    return nodo->izquierdo->tipo;
                }
            }
        }else if(nodo->derecho->tipo == SUMA_NODO || nodo->derecho->tipo == RESTA_NODO){
            return comprobarOperaciones(nodo->derecho, errores, pila);
        }
    }
    return -1;
}

/*
    Asigna el tipo de dato a la variable en la tabla de símbolos
*/
int asignarValor(NodoAST* nodo, struct ArrayError* errores, struct Pila* pila){
    if(nodo != NULL){
        if(nodo->tipo == INT_NODO){
            if(nodo->izquierdo == NULL)
                return INT_NODO;
        }else if(nodo->tipo == CHAR_NODO){
            if(nodo->izquierdo == NULL)
                return CHAR_NODO;
        }else if(nodo->tipo == SUMA_NODO || nodo->tipo == RESTA_NODO){
            return comprobarOperaciones(nodo, errores, pila);
        }else if(nodo->tipo == ID_NODO){
            Simbolo aux = buscarSimbolo(pila, nodo->lexema);
            return aux.tipo;
        }
    }
    return -1;
}

/*
    Revisa el tipo de dato de cada variable y que tengan coherencia
*/
void semantico2(NodoAST* nodo, int nivel, struct Pila* pila, struct ArrayError* errores){
    if(nodo != NULL){
        struct Simbolo aux;
        int contadorErrores = errores->numErrores;
        if(nodo->tipo == ASIGNACION_NODO){
            pila->tope->simbolo.tipo = asignarValor(nodo->derecho, errores, pila);
            if(pila->tope->simbolo.tipo == -1){
                errores->numErrores = ++contadorErrores;
                    if(contadorErrores == 1){
                        errores->error = (struct Error*)malloc(sizeof(struct Error));
                    }else{
                        struct Error* temp = (struct Error*)realloc(errores->error, (contadorErrores)*sizeof(struct Error));
                    }
                    errores->error[contadorErrores-1].tipo = SEMANTICO_TI;
                    strcpy(errores->error[contadorErrores-1].lexema, nodo->lexema);
                    errores->error[contadorErrores-1].linea = nodo->linea;
            }
        }
    }
}