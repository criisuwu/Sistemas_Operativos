#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insertion_sort(char **str, int n) {
    for (int i = 1; i < n; i++) {
        char *key = str[i];
        int j = i - 1;

        while (j >= 0 && strcmp(str[j], key) > 0) {
            str[j + 1] = str[j];
            j--;
        }
        str[j + 1] = key;
    }
}

int main(int argc, char **argv){
    int n = argc - 1;

    if(argc <= 1){
        perror("Se requieren argumentos");
        exit(EXIT_FAILURE); //Porque cuando termina me pone success?
    }

    char **list = malloc(sizeof(char *) * (argc - 1));
    if(!list)
    {
        free(list);
        perror("Fallo al alocar memoria");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++){
        int word = strlen(argv[i]); //No estoy segura de si puedo implementar esta funcion
        list[i-  1] = malloc(sizeof(char)* word + 1);
        if(!list)
        {
            free(list);
            perror("Fallo al alocar memoria");
            exit(EXIT_FAILURE);
        }

        for(int k = 0; k < word; k++){
            char c = argv[i][k];

            if(c >= 'a' && c <= 'z')
                list[i-1][k] = c - 32;
            else
                list[i-1][k] = c;
        }
        list[i-1][word] = '\0';
    }

    insertion_sort(list, n);

    for (int i = 0; i < n; i++) {
        if (i == 0 || strcmp(list[i], list[i - 1]) != 0)
            printf("%s\n", list[i]);
    }

    for(int i=0;i<argc-1;i++)
        free(list[i]);
    free(list);

    exit(EXIT_SUCCESS);
}