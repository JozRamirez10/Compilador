/*
    Construye el árbol AST
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum{
    ASIGNACION_NODO,
    SUMA_NODO,
    RESTA_NODO,
    ID_NODO,
    INT_NODO,
} TipoNodo;

typedef struct Nodo{
    TipoNodo tipo;
    char lexema[20];
    struct Nodo* izquierdo;
    struct Nodo* derecho;
}Nodo;

/*
    Declaración de funciones
*/
Nodo* nodoS(struct Token* tokens);
Nodo* nodoA(struct Token* tokens);
Nodo* nodoB(struct Token* tokens);
Nodo* nodoB_parentesis(struct Token* tokens);
Nodo* nodovalidarParentesis(struct Token* tokens);
Nodo* nodoC(struct Token* tokens);

int contadorNodo = 0; // Contador global que itera sobre el arreglo de tokens

Nodo* crearNodo(TipoNodo tipo, const char* lexema){
    Nodo* nodo = (Nodo*)malloc(sizeof(Nodo));
    nodo->tipo = tipo;
    strncpy(nodo->lexema, lexema, sizeof(nodo->lexema)-1);
    nodo->lexema[sizeof(nodo->lexema)-1] = '\0';
    nodo->izquierdo = NULL;
    nodo->derecho = NULL;
    return nodo;
}

void imprimirNodo(Nodo* nodo, int nivel){
    if(nodo != NULL){
        for(int i = 0; i < nivel; i++){
            printf(" ");
        }
        printf("Tipo: %d, Lexema: %s\n", nodo->tipo, nodo->lexema);

        imprimirNodo(nodo->izquierdo, nivel + 1);
        imprimirNodo(nodo->derecho, nivel + 1);
    }
}

void liberarNodo(Nodo* nodo){
    if(nodo!= NULL)
        free(nodo);
}

/*
    Comprueba que se cumpla la regla C
*/
Nodo* nodoC(struct Token* tokens){
    Nodo* nodo = NULL;
    if(tokens[contadorNodo].tipo == ID){
        nodo = crearNodo(ID_NODO, tokens[contadorNodo].lexema);
        contadorNodo++;
    }   
    if(tokens[contadorNodo].tipo == INT){
        nodo = crearNodo(INT_NODO, tokens[contadorNodo].lexema);
    }
    return nodo;
}

/*
    Comprueba que dentro del paréntesis haya producciones
    válidas y que además se cierre
*/
Nodo* nodovalidarParentesis(struct Token* tokens){
    Nodo* nodo = NULL;
    if(tokens[contadorNodo].tipo == PARENT_IZQ){
        contadorNodo++;
        nodo = nodoB_parentesis(tokens);
        if(!(tokens[contadorNodo].tipo == PARENT_DER)){
            liberarNodo(nodo);
            nodo = NULL;
        }
    }
    return nodo;
}

/*
    Comprueba que se cumpla la regla B para producciones
    dentro de un paréntesis
*/
Nodo* nodoB_parentesis(struct Token* tokens){
    Nodo* izquierdo = nodoC(tokens);
    if(izquierdo != NULL){
        contadorNodo++;
        if(tokens[contadorNodo].tipo == SUMA || tokens[contadorNodo].tipo == RESTA){
            TipoNodo tipo = (tokens[contadorNodo].tipo == SUMA) ? SUMA_NODO : RESTA_NODO;
            Nodo* nodo = crearNodo(tipo, tokens[contadorNodo].lexema);
            contadorNodo++;
            Nodo* derecho = nodoB_parentesis(tokens);
            if(derecho != NULL){
                nodo->izquierdo = izquierdo;
                nodo->derecho = derecho;
                return nodo;
            }else{
                liberarNodo(nodo);
            }
        }else{
            return izquierdo;
        }
    }
    return NULL;
}

/*
    Comprueba que se cumpla la regla B
*/
Nodo* nodoB(struct Token* tokens){
    Nodo* izquierdo = nodoC(tokens);
    if(izquierdo != NULL){ // Válida si es ID|INT
        contadorNodo++;
        if(tokens[contadorNodo].tipo == SUMA || tokens[contadorNodo].tipo == RESTA){
            TipoNodo tipo = (tokens[contadorNodo].tipo == SUMA ? SUMA_NODO : RESTA_NODO);
            Nodo* nodo = crearNodo(tipo, tokens[contadorNodo].lexema);
            contadorNodo++;
            Nodo* derecho = nodoB(tokens);
            if(derecho != NULL){
                nodo->izquierdo = izquierdo;
                nodo->derecho = derecho;
                return nodo;
            }else{
                liberarNodo(nodo);
            }
        }else{
            return izquierdo;
        }
    }
    Nodo* parentesis = nodovalidarParentesis(tokens);
    if(parentesis != NULL){
        contadorNodo++;
        if(tokens[contadorNodo].tipo == SUMA || tokens[contadorNodo].tipo == RESTA){
            TipoNodo tipo = (tokens[contadorNodo].tipo == SUMA ? SUMA_NODO : RESTA_NODO);
            Nodo* nodo = crearNodo(tipo, tokens[contadorNodo].lexema);
            contadorNodo++;
            Nodo* derecho = nodoB(tokens);
            if(derecho != NULL){
                nodo->izquierdo = izquierdo;
                nodo->derecho = derecho;
                return nodo;
            }else{
                liberarNodo(nodo);
            }
        }else{
            return parentesis;
        }
    }
    return NULL;
}

/*
    Comprueba que se cumpla la regla A
*/
Nodo* nodoA(struct Token* tokens){
    Nodo* izquierda = nodoB(tokens);
    if(izquierda != NULL){
        if(tokens[contadorNodo].tipo == FIN){
            return izquierda;
        }
    }
    return NULL;
}

/*
    Comprueba que se cumpla la regla S
*/
Nodo* nodoS(struct Token* tokens){
    if(tokens[contadorNodo].tipo == ID){
        Nodo* nodo = crearNodo(ID_NODO, tokens[contadorNodo].lexema);
        contadorNodo++;
        if(tokens[contadorNodo].tipo == ASIGNACION){
            Nodo* asignacion = crearNodo(ASIGNACION_NODO, "=");
            contadorNodo++;
            Nodo* expresionNodo = nodoA(tokens);
            asignacion->izquierdo = nodo;
            asignacion->derecho = expresionNodo;
            return asignacion;
        }else{
            return nodo;
        }
    }else{
        return NULL;
    }
}

/*
    Itera sobre los tokens para comprobar que cumpla las reglas
    de la gramática
*/
void ast(struct Token* tokens, int cantidad){
    Nodo* raizAST;
    while(contadorNodo < cantidad){
        raizAST = nodoS(tokens);
        imprimirNodo(raizAST, 0);
        contadorNodo++;
    }
    liberarNodo(raizAST);
}