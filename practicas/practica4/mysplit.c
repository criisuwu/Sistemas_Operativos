/*
*   Ejercicio 4. mysplit
*
*   Este programa simula el funcionamiento de la funcion split. La cual
*   consiste en que reciba un tamaño N de bytes y un fichero para poder
*   hacer una separacion que sera guardada en diferentes archivos nuevos
*   con el tamaño N recibido en la ejecucion. En caso de que el archivo
*   o archivos que se van a crear ya existieran estos se truncaran y se
*   se sobre escribiran.
*
*   Creado por: Cristina Homobono Fernández
*   Fecha: 10 abril 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

/* Funcion que imprime error de uso */
void
usage(char *progname)
{
	fprintf(stderr, "usage: %s N file\n", progname);
	exit(EXIT_FAILURE);
}

/* Funcion para parsear y validar el argumento N */
long
parse_size(char *str)
{
	char *endptr;
	long N = strtol(str, &endptr, 10);

	if (*endptr != '\0' || N <= 0) {
		fprintf(stderr, "Error: invalid size\n");
		exit(EXIT_FAILURE);
	}
	return N;
}

/* Funcion que abre el fichero de entrada */
int
open_input(char *filename)
{
	int fd = open(filename, O_RDONLY);

	if (fd < 0) {
		fprintf(stderr, "Error: couldn't open the file\n");
		exit(EXIT_FAILURE);
	}
	return fd;
}

/* Lee hasta N bytes del fd y devuelve los bytes leídos */
long
read_chunk(int fd, char *buf, long N)
{
	long bytes = 0;

	while (bytes < N) {
		long r = read(fd, buf + bytes, N - bytes);

		if (r < 0)
			return -1;
		if (r == 0)
			break;
		bytes += r;
	}
	return bytes;
}

/* Escribe los bytes */
int
write_chunk(int fd, char *buf, long bytes_in_buf)
{
	long written = 0;

	while (written < bytes_in_buf) {
		long w = write(fd, buf + written, bytes_in_buf - written);

		if (w < 0)
			return -1;
		written += w;
	}
	return 0;
}

/* Abre o crea el fichero de salida */
int
open_output(int part, char *filename, char *outname, int filename_len)
{
	snprintf(outname, filename_len, "%03d%s", part, filename);
	int fd = open(outname, O_WRONLY | O_CREAT | O_TRUNC, 0664);

	return fd;
}

/* Simula el funcionamiento del split */
void
ft_split(int fd_in, char *filename, long N)
{
	char *buf = malloc(N);

	if (!buf) {
		fprintf(stderr, "ERROR: malloc failed\n");
		close(fd_in);
		exit(EXIT_FAILURE);
	}

	int filename_len = 3 + strlen(filename) + 1;
	char *outname = malloc(filename_len);

	if (!outname) {
		fprintf(stderr, "ERROR: malloc failed\n");
		free(buf);
		close(fd_in);
		exit(EXIT_FAILURE);
	}

	int part = 0;

	while (1) {
		long bytes = read_chunk(fd_in, buf, N);

		if (bytes < 0) {
			fprintf(stderr, "ERROR: read error\n");
			free(buf);
			free(outname);
			close(fd_in);
			exit(EXIT_FAILURE);
		}
		if (bytes == 0)
			break;

		int fd_out = open_output(part, filename, outname, filename_len);

		if (fd_out < 0) {
			fprintf(stderr, "ERROR: cannot open output file\n");
			free(buf);
			free(outname);
			close(fd_in);
			exit(EXIT_FAILURE);
		}

		if (write_chunk(fd_out, buf, bytes) < 0) {
			fprintf(stderr, "ERROR: write error\n");
			free(buf);
			free(outname);
			close(fd_in);
			close(fd_out);
			exit(EXIT_FAILURE);
		}

		close(fd_out);
		part++;
	}

	free(buf);
	free(outname);
}

int
main(int argc, char *argv[])
{
	if (argc != 3)
		usage(argv[0]);

	long N = parse_size(argv[1]);
	int fd_in = open_input(argv[2]);

	ft_split(fd_in, argv[2], N);

	close(fd_in);
	exit(EXIT_SUCCESS);
}
