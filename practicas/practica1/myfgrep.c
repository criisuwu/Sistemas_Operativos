/*
*   Ejercicio 1. Myfgrep
*
*   El programa esta preparado para recibir argumentos, donde el primero es el ejecutable
*   el segundo es una palabra a buscar (en caso de haber cuatro, la tercera seria tambien
*   otra palabra a buscar) y la ultima seria la ruta al archivo o solo el archivo sobre el
*   cual estamos ejecutando el fgrep. Además de eso, debe de imprimir la coincidencia e 
*   imprimirla una sola vez.
*
*   Creado por: Cristina Homobono Fernández
*   Fecha: 6 marzo 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Valida argumentos
*       ¡ANOTACIÓN!: Para verificar que el archivo existe o que no tiene permisos
*       de lectura, seria necesario hacer uso del comando open() y de la flag
*       O_RDONLY.
*       Por ende, los archivos inexistentes o sin permiso de lectura daran pie
*       a la salida de error del grep.
*/
void validate_args(int argc) {
    if (argc < 3) {
        fprintf(stderr, "usage: myfgrep word [word]... file\n");
        exit(EXIT_FAILURE);
    }
}

/* Construye el array de argumentos para pasarle a execvp
*  y asi poder ejecutar el comando fgrep.
*/
char **build_command(int argc, char *argv[]) {
    int words = argc - 2;
    int len = 1 + 1 + 2 * words + 1 + 1;

    char **command = malloc(len * sizeof(char *));
    if (!command) {
        fprintf(stderr, "myfgrep: malloc error\n");
        exit(EXIT_FAILURE);
    }

    char *filepath = argv[argc - 1];
    int pos = 0;

    command[pos++] = "fgrep";
    command[pos++] = "-n";
    for (int i = 0; i < words; i++) {
        command[pos++] = "-e";
        command[pos++] = argv[i + 1];
    }
    command[pos++] = filepath;
    command[pos]   = NULL;

    return command;
}

/* Hace fork y ejecuta fgrep con el comando dado*/
void run_fgrep(char **command) {
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "myfgrep: fork error\n");
        free(command);
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        execvp("fgrep", command);
        fprintf(stderr, "myfgrep: execvp error\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    validate_args(argc);

    char **command = build_command(argc, argv);
    run_fgrep(command);

    free(command);
    exit(EXIT_SUCCESS);
}