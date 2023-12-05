# Compilador Team 12

Instrucciones 
* Compilación
  ```
  gcc -o main main.c
  ```
* Ejecución
  Al ejecutar el programa, éste debe recibir como argumento la ruta de un archivo que contenga los lexemas.
  ```
  ./main test/test1
  ```

## Analizador Léxico

El analizador léxico consta de dos parte fundamentales dentro del código.
  - gramatica.c : Se encarga de verificar que la gramática se cumpla:
    ```
    asignación => alphanum | expresion
    alphanum => id | numero
    id => [a-zA-Z][a-zA-Z0-9]*
    numero => [0-9]+
    expresion => alphanum + expresion | alphanum - expresion | (expresion)
    ```
  - analizador.c : Una vez que se ha válidado la grámatica, el analizador determina el tipo de token de cada lexema.
    
Entrada:
  ```
  x = 2
  ```

Salida:
  ```
  IDENTIFICADOR ASIGNACION NUMERO FIN
  ```

Nota: Los archivos de código y pruebas están dentro de la carpeta "Analizador lexico"

## Analizador sintáctico (Parser)

El parser recibe los tokens del analizador léxico y los válida de acuerdo a las reglas de la gramática.
  - La gramática inicial se adapto a la siguiente de acuerdo a los tokens:
    ```
    S => ID = A
    A => B | C
    B => C + B | C - B | (B) | C
    C => ID | INT
    ID => [a-zA-Z][a-zA-Z0-9]*
    INT => [0-9]+
    ```  
Entrada 1:
  ```
  x = 2 + 1;
  ```
Salida 2:
  ```
  Valid
  ```

Entrada 2:
  ```
  x = 2 + ;
  ```
Salida 2:
  ```
  Error
  ```
Nota: Los archivos de código y pruebas están dentro de la carpeta "Parser"

## Analizador semántico 
### AST, tabla de símbolos, manejo de errores y generación de código

  - Para que el analizador semántico pueda revisar la coherencia del código fuente y realizar comparaciones entre diferentes tipos, se decidió agregar a la gramática la variable de tipo CHAR.
    ```
    S => ID = A | ID = CHAR
    A => B | C
    B => C + B | C - B | (B) | C
    C => ID | INT
    ID => [a-zA-Z][a-zA-Z0-9]*
    INT => [0-9]+
    CHAR => '[a-zA-Z]'
    ```

Cuando el compilador válida el código fuente (sin encontrar errores) procede a generar un archivo con extensión "obj" en lenguaje ensamblador con respectos a las sentencias dadas.

Entrada:
  ```
  x = 2;
  ```
Salida:
  ```
  section .data
    x dd 0
  section .text
    global main
  main:
    mov dword [x], 2

    mov eax, 0
    ret
  ```

Si encuentra errores, los clasifica e imprime en pantalla.

Entrada
  ```
  25;
  x = ;
  2 - 4
  \;
  ```

Salida
  ```
  Linea 4 | (Error léxico) Token sin reconocer: \
  Linea 1 | (Error sintáctico) No cumple con la gramatica: 25 ; 
  Linea 2 | (Error sintáctico) No cumple con la gramatica: x = ; 
  Linea 4 | (Error sintáctico) No cumple con la gramatica: ; 
  ```
Nota: Los archivos de código y pruebas están dentro de la carpeta "Compilador"



