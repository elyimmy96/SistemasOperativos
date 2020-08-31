# SistemasOperativos

## SORT
El programa sort ordena utilizando threads un arreglo de n números aleatorios.
Para compilarlo es necesario correr el comando:
    ```
    gcc -o sort.c -lpthread -lncurses sort.c
    ```  
* pthread es utilizada para crear los threads
* ncurses es utilizada para crear la interfaz de usuario

Para correr el programa se pueden utilizar dos posibles comandos:
*`./sort` -> simplemente corre el programa, escribiendo en pantalla el tiempo por thread y el tiempo final.
*`./sort -v` -> corre el programa imprimiendo el arreglo original y el arreglo ordenado
