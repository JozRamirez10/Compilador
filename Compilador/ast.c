/*
    Construye el árbol AST
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
    Declaración de funciones
*/
NodoAST* nodoS(struct Token* tokens);
NodoAST* nodoA(struct Token* tokens);
NodoAST* nodoB(struct Token* tokens);
NodoAST* nodoB_parentesis(struct Token* tokens);
NodoAST* nodovalidarParentesis(struct Token* tokens);
NodoAST* nodoC(struct Token* tokens);

int contadorNodo = 0; // Contador global que itera sobre el arreglo de tokens

NodoAST* crearNodo(TipoNodo tipo, const char* lexema, int linea){
    NodoAST* nodo = (NodoAST*)malloc(sizeof(NodoAST));
    nodo->tipo = tipo;
    strncpy(nodo->lexema, lexema, sizeof(nodo->lexema)-1);
    nodo->lexema[sizeof(nodo->lexema)-1] = '\0';
    nodo->linea = linea;
    nodo->izquierdo = NULL;
    nodo->derecho = NULL;
    return nodo;
}

void imprimirNodo(NodoAST* nodo, int nivel){
    if(nodo != NULL){
        for(int i = 0; i < nivel; i++){
            printf(" ");
        }
        printf("%s\n", nodo->lexema);

        imprimirNodo(nodo->izquierdo, nivel + 1);
        imprimirNodo(nodo->derecho, nivel + 1);
    }
}

void liberarNodo(NodoAST* nodo){
    if(nodo!= NULL)
        free(nodo);
}

/*
    Comprueba que se cumpla la regla C
*/
NodoAST* nodoC(struct Token* tokens){
    NodoAST* nodo = NULL;
    if(tokens[contadorNodo].tipo == ID){
        nodo = crearNodo(ID_NODO, tokens[contadorNodo].lexema, tokens[contadorNodo].linea);
    }   
    if(tokens[contadorNodo].tipo == INT){
        nodo = crearNodo(INT_NODO, tokens[contadorNodo].lexema, tokens[contadorNodo].linea);
    }
    return nodo;
}

/*
    Comprueba que dentro del paréntesis haya producciones
    válidas y que además se cierre
*/
NodoAST* nodovalidarParentesis(struct Token* tokens){
    NodoAST* nodo = NULL;
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
NodoAST* nodoB_parentesis(struct Token* tokens){
    NodoAST* izquierdo = nodoC(tokens);
    if(izquierdo != NULL){
        contadorNodo++;
        if(tokens[contadorNodo].tipo == SUMA || tokens[contadorNodo].tipo == RESTA){
            TipoNodo tipo = (tokens[contadorNodo].tipo == SUMA) ? SUMA_NODO : RESTA_NODO;
            NodoAST* nodo = crearNodo(tipo, tokens[contadorNodo].lexema, tokens[contadorNodo].linea);
            contadorNodo++;
            NodoAST* derecho = nodoB_parentesis(tokens);
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
NodoAST* nodoB(struct Token* tokens){
    NodoAST* izquierdo = nodoC(tokens);
    if(izquierdo != NULL){ // Válida si es ID|INT
        contadorNodo++;
        if(tokens[contadorNodo].tipo == SUMA || tokens[contadorNodo].tipo == RESTA){
            TipoNodo tipo = (tokens[contadorNodo].tipo == SUMA ? SUMA_NODO : RESTA_NODO);
            NodoAST* nodo = crearNodo(tipo, tokens[contadorNodo].lexema, tokens[contadorNodo].linea);
            contadorNodo++;
            NodoAST* derecho = nodoB(tokens);
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
    NodoAST* parentesis = nodovalidarParentesis(tokens);
    if(parentesis != NULL){
        contadorNodo++;
        if(tokens[contadorNodo].tipo == SUMA || tokens[contadorNodo].tipo == RESTA){
            TipoNodo tipo = (tokens[contadorNodo].tipo == SUMA ? SUMA_NODO : RESTA_NODO);
            NodoAST* nodo = crearNodo(tipo, tokens[contadorNodo].lexema, tokens[contadorNodo].linea);
            contadorNodo++;
            NodoAST* derecho = nodoB(tokens);
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
    Comprueba que el nodo sea un CHAR
*/
NodoAST* nodoCHR(struct Token* tokens){
    NodoAST* nodo = NULL;
    if(tokens[contadorNodo].tipo == CHAR){
        nodo = crearNodo(CHAR_NODO, tokens[contadorNodo].lexema, tokens[contadorNodo].linea);
    }
    return nodo;
}

/*
    Comprueba que se cumpla la regla A
*/
NodoAST* nodoA(struct Token* tokens){
    NodoAST* izquierda = nodoB(tokens);
    if(izquierda != NULL){
        if(tokens[contadorNodo].tipo == FIN){
            return izquierda;
        }
    }else{
        NodoAST* izquierda = nodoCHR(tokens);
        if(izquierda != NULL){
            contadorNodo++;
            if(tokens[contadorNodo].tipo == FIN){
                return izquierda;
            }
        }
    }
    return NULL;
}

/*
    Comprueba que se cumpla la regla S
*/
NodoAST* nodoS(struct Token* tokens){
    if(tokens[contadorNodo].tipo == ID){
        NodoAST* nodo = crearNodo(ID_PRINCIPAL, tokens[contadorNodo].lexema, tokens[contadorNodo].linea);
        contadorNodo++;
        if(tokens[contadorNodo].tipo == ASIGNACION){
            NodoAST* asignacion = crearNodo(ASIGNACION_NODO, "=", tokens[contadorNodo].linea);
            contadorNodo++;
            NodoAST* expresionNodo = nodoA(tokens);
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
void construccionAST(struct Token* tokens, int cantidad, struct Pila* pila, struct ArrayError* errores){
    NodoAST* raizAST;
    int contadorErrores = errores->numErrores;
    while(contadorNodo < cantidad){
        raizAST = nodoS(tokens); // Obtiene el nodo AST
        semantico1(raizAST, 0, pila, errores); // Análisis semántico (declaración de variables)
        if(errores->numErrores == contadorErrores){ // Verifica errores
            contadorErrores = errores->numErrores;
            semantico2(raizAST, 0, pila, errores); // Análsis semantico 2 (Coherencia entre datos)
            if(errores->numErrores == contadorErrores){ // Verifica errores
                generaCodigo(raizAST); // Genera el código ensamblador
            }
        }
        contadorNodo++;
    }
    
    liberarNodo(raizAST);
}