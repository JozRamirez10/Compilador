/*
    Estructura del AST
*/
typedef enum{
    ASIGNACION_NODO, // 0
    SUMA_NODO, // 1
    RESTA_NODO, // 2
    ID_NODO, // 3
    INT_NODO, // 4
    ID_PRINCIPAL, // 5
    CHAR_NODO, // 6
} TipoNodo;

// -1 -> Tipo inválido

typedef struct NodoAST{
    TipoNodo tipo;
    char lexema[20];
    int linea;
    struct NodoAST* izquierdo;
    struct NodoAST* derecho;
}NodoAST;