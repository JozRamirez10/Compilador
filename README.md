# Analizador léxico

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
  
Entrada:
  ```
  x = 2
  ```

Salida:
  ```
  IDENTIFICADOR ASIGNACION NUMERO FIN
  ```

Nota: Los archivos de código y pruebas están dentro de la carpeta "Analizador lexico"

