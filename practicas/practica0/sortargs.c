/*
Ejercicio 0. Sortargs

El programa esta preparado para recibir argumentos, con ello transforma las
minusculas a mayusculas, y si es cualquier otro caracter imprimible o numero lo
deja tal y como esta para luego ordenarlos lexicamente de forma creciente.
Además, evita que se impriman palabras repetidas.

Creado por: Cristina Homobono Fernández
Fecha: 20 febrero 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funcion de verificacion de argumentos
void
check_arg(int argc)
{
	if (argc <= 1) {
		fprintf(stderr, "Error: se requieren argumnetos");
		exit(EXIT_FAILURE);
	}
}

// Funcion para cambiar a mayuscula o ignorar mayusculas y otros caracteres
void
upcase_change(char **list, char c, int i, int k)
{
	if (c >= 'a' && c <= 'z')
		list[i - 1][k] = c - 32;
	else
		list[i - 1][k] = c;
}

// Funcion del algoritmo de ordenacion
void
insertion_sort(char **str, int n)
{
	for (int i = 1; i < n; i++) {
		char *word = str[i];
		int j = i - 1;

		while (j >= 0 && strcmp(str[j], word) > 0) {
			str[j + 1] = str[j];
			j--;
		}
		str[j + 1] = word;
	}
}

// Funcion para evitar que las palabras se repitan
void
print_once(int n, char **list)
{
	for (int i = 0; i < n; i++) {
		if (i == 0 || strcmp(list[i], list[i - 1]) != 0)
			printf("%s\n", list[i]);
	}

}

// Funcion para liberar la memoria
void
free_list(int argc, char **list)
{
	for (int i = 0; i < argc - 1; i++)
		free(list[i]);
	free(list);
}

int
main(int argc, char **argv)
{
	int n = argc - 1;
	char **list = malloc(sizeof(char *) * (argc - 1));

	check_arg(argc);

	if (!list) {
		free(list);
		fprintf(stderr, "Error: Fallo de reserva de memoria");
		exit(EXIT_FAILURE);
	}

	for (int i = 1; i < argc; i++) {
		int word = strlen(argv[i]);

		list[i - 1] = malloc(sizeof(char) * word + 1);
		if (!list) {
			free(list);
			fprintf(stderr, "Error: Fallo de reserva de memoria");
			exit(EXIT_FAILURE);
		}

		for (int k = 0; k < word; k++) {
			char c = argv[i][k];

			upcase_change(list, c, i, k);
		}
		list[i - 1][word] = '\0';
	}

	insertion_sort(list, n);
	print_once(n, list);
	free_list(argc, list);

	exit(EXIT_SUCCESS);
}
