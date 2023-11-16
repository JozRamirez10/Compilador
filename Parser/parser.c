/*
    Analiza que se cumpla la grámatica con los tokens que recibe
    del analizador léxico

    S -> ID = A
    A -> B | C
    B -> C + B | C - B | ( B ) | C + C | C - C | C
    C -> ID | INT 
    ID -> [a-zA-Z][a-zA-Z0-9]*
    INT -> [0-9]+
*/

#include <stdio.h>
#include <stdbool.h>

/*
    Declaración de funciones
*/
bool S(struct Token* tokens);
bool A(struct Token* tokens);
bool B(struct Token* tokens);
bool B_parentesis(struct Token* tokens);
bool validarParentesis(struct Token* tokens);
bool C(struct Token* tokens);

int contador = 0; // Contador global que itera sobre el arreglo de tokens

/*
    Comprueba que se cumpla la regla C
*/
bool C(struct Token* tokens){
    if(tokens[contador].tipo == ID){
        contador++;
        return true;
    }   
    if(tokens[contador].tipo == INT){
        contador++;
        return true;
    }
    return false;
}

/*
    Comprueba que dentro del paréntesis haya producciones
    válidas y que además se cierre
*/
bool validarParentesis(struct Token* tokens){
    if(tokens[contador].tipo == PARENT_IZQ){
        contador++;
        if(B_parentesis(tokens)){
            return true;
        }
    }
    return false;
}

/*
    Comprueba que se cumpla la regla B para producciones
    dentro de un paréntesis
*/
bool B_parentesis(struct Token* tokens){
    if(C(tokens))
        switch (tokens[contador].tipo){
        case SUMA:
            contador++;
            if(B_parentesis(tokens))
                return true;
            break;
        case RESTA:
            contador++;
            if(B_parentesis(tokens))
                return true;
            break;
        case PARENT_DER:
            contador++;
            return true;
            break;
    }
    return false;
}

/*
    Comprueba que se cumpla la regla B
*/
bool B(struct Token* tokens){
    if(C(tokens)) // Válida si es ID|INT
        switch (tokens[contador].tipo){
        case SUMA: // Válida si es suma
            contador++;
            if(B(tokens))
                return true;
            break;
        case RESTA: // Valida si es resta
            contador++;
            if(B(tokens))
                return true;
            break;
        default: // En cualquier otro caso, válida C
            return true;
            break; 
    }
    if(validarParentesis(tokens)) // Valida los paréntesis
        return true;
    return false;
}

/*
    Comprueba que se cumpla la regla A
*/
bool A(struct Token* tokens){
    if(B(tokens)){
        if(tokens[contador].tipo == FIN)
            return true;
    }
    return false;
}

/*
    Comprueba que se cumpla la regla S
*/
bool S(struct Token* tokens){
    if(tokens[contador].tipo == ID){
        contador++;
        if(tokens[contador].tipo == ASIGNACION){
            contador++;
            if(A(tokens))
                return true;
        }
    }
    return false;
}

/*
    Itera sobre los tokens para comprobar que cumpla las reglas
    de la gramática
*/
bool parser(struct Token* tokens, int cantidad){

    while(contador < cantidad){
        if(!S(tokens)){
            return false;
        }
        contador++;
    }
    return true; 
}