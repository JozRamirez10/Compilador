// Estructura para almacenar información sobre los tokens
struct Token {
    int tipo;
    char lexema[100];
    int linea;
};

// Definición de tokens
enum tken{
    ID,         // 0
    IF,         // 1
    ELSE,       // 2
    WHILE,      // 3
    INT,        // 4
    FLOAT,      // 5
    ASIGNACION, // 6
    SUMA,       // 7
    RESTA,      // 8
    MULT,       // 9
    DIV,        // 10
    PARENT_IZQ, // 11
    PARENT_DER, // 12
    KEY_IZQ,    // 13
    KEY_DER,    // 14
    COMA,       // 15
    FIN,        // 16
    COMENTARIO, // 17
    ERROR,      // 18
    SALTO,      // 19
    ESPACIO,    // 20
    COMILLA,
    CHAR,
};