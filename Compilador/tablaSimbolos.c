#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Simbolo{
    int tipo;
    char lexema[20];
} Simbolo;

typedef struct NodoPila {
    Simbolo simbolo;
    struct NodoPila* sig;
} NodoPila;

typedef struct Pila{
    NodoPila* tope;
} Pila;

void inicializarPila(Pila* pila);
void push(Pila* pila, Simbolo simbolo);
void pop(Pila* pila);
Simbolo buscarSimbolo(Pila* pila, const char* lexema);

void inicializarPila(Pila* pila){
    pila->tope = NULL;
}

void push(Pila* pila, Simbolo simbolo){
    NodoPila* nuevoNodo = (NodoPila*)malloc(sizeof(NodoPila));
    if(nuevoNodo == NULL){
        exit(EXIT_FAILURE);
    }
    nuevoNodo->simbolo = simbolo;
    nuevoNodo->sig = pila->tope;
    pila->tope = nuevoNodo;
}

void pop(Pila* pila){
    if(pila->tope == NULL){
        exit(EXIT_FAILURE);
    }
    NodoPila* nodoPop = pila->tope;
    pila->tope = nodoPop->sig;
    //printf("Desacoplado: %s, tipo: %d\n", nodoPop->simbolo.lexema, nodoPop->simbolo.tipo);
    free(nodoPop);
}

Simbolo buscarSimbolo(Pila* pila, const char* lexema){
    NodoPila* actual = pila->tope;
    while(actual != NULL){
        if(strcmp(actual->simbolo.lexema, lexema) == 0){
            return actual->simbolo;
        }
        actual = actual -> sig;
    }
    Simbolo invalido;
    strcpy(invalido.lexema, "NULL");
    return invalido; 
}

