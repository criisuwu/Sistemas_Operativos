/*
*   Ejercicio 5. curler
*
*   El objetivo de este programa es que pueda buscar, verificar y descargar
*   las URL pasadas al programa en un fichero. Si la URL no existe o no se
*   puede descargar dara error. En este caso el programa solo puede recibir
*   un argumento, en caso contrario dara error. Ademas, el maximo de las
*   lineas del archivo son 512 y en caso de que no empezaran por https://
*   o http:// dara error. Un timeout de 5 segundos es obligatorio.
*
*   Creado por: Cristina Homobono Fernández
*   Fecha: 16 abril 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void
run_curl(const char *url)
{
	FILE *devnull = fopen("/dev/null", "w");

	if (!devnull)
		exit(1);
	int fd = fileno(devnull);

	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	execl("/usr/bin/curl", "curl", "--connect-timeout", "5", url,
	      (char *)NULL);
	execl("/bin/curl", "curl", "--connect-timeout", "5", url, (char *)NULL);
	exit(1);
}

int
check_url(const char *url)
{
	pid_t pid = fork();

	if (pid < 0) {
		perror("fork");
		exit(255);
	}
	if (pid == 0)
		run_curl(url);
	int status;

	waitpid(pid, &status, 0);
	return !(WIFEXITED(status) && WEXITSTATUS(status) == 0);
}

FILE *
open_input(int argc, char *argv[])
{
	if (argc > 2) {
		fprintf(stderr, "error: too many arguments\n");
		exit(255);
	}
	if (argc == 2) {
		FILE *f = fopen(argv[1], "r");

		if (!f) {
			perror("error opening file");
			exit(255);
		}
		return f;
	}
	return stdin;
}

char *
alloc_line()
{
	char *line = malloc(514);

	if (!line) {
		fprintf(stderr, "error: out of memory\n");
		exit(255);
	}
	return line;
}

void
validate_line(const char *line, size_t len, char *buf, FILE *input)
{
	if (len > 512 ||
	    (strncmp(line, "http://", 7) != 0
	     && strncmp(line, "https://", 8) != 0)) {
		fprintf(stderr, "error: invalid line \"%s\"\n", line);
		free(buf);
		if (input != stdin)
			fclose(input);
		exit(255);
	}
}

int
process_urls(FILE *input)
{
	char *line = alloc_line();
	int failures = 0;

	while (fgets(line, 514, input) != NULL) {
		size_t len = strlen(line);

		if (len > 0 && line[len - 1] == '\n')
			line[--len] = '\0';
		validate_line(line, len, line, input);
		if (check_url(line) != 0)
			failures++;
	}

	free(line);
	if (failures > 255)
		failures = 255;
	return failures;
}

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "ERROR: not enough arguments\n");
		exit(EXIT_FAILURE);
	} else {
		FILE *input = open_input(argc, argv);
		int failures = process_urls(input);

		if (input != stdin)
			fclose(input);
		if (failures == 0)
			exit(EXIT_SUCCESS);
		exit(failures);
	}
}
