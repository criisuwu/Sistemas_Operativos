/*
*   Ejercicio 5. catrecurtxt
*
*   El objetivo de este programa es buscar en todos los directorios y subdirectorios
*   archivos que terminen en .txt y guardar el texto contenido en esos archivos en un
*   archivo de salida. El archivo de salida puede ser un unico fichero (entonces lo
*   crea en el directorio actual) o puede recibir una ruta y un archivo (Entonces
*   crea el fichero en esa ruta).
*
*   Creado por: Cristina Homobono Fernández
*   Fecha: 24 abril 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/* Esta funcion comprueba que el nombre del fichero termine en .txt */
static int
ends_with_txt(const char *filename)
{
	size_t length = strlen(filename);

	if (length < 4)
		return 0;
	return strcmp(filename + length - 4, ".txt") == 0;
}

/* Esta funcion construye la ruta concatenando el directorio y el, nombre del fichero */
static char *
build_path(const char *directory, const char *filename)
{
	size_t directory_length = strlen(directory);
	size_t filename_length = strlen(filename);
	char *path = malloc(directory_length + 1 + filename_length + 1);

	if (!path)
		return NULL;
	memcpy(path, directory, directory_length);
	path[directory_length] = '/';
	memcpy(path + directory_length + 1, filename, filename_length + 1);
	return path;
}

/* Esta funcion copia el contenido del fichero al fichero de salida */
static int
copy_file(const char *source, int output_fd)
{
	int input_fd = open(source, O_RDONLY);

	if (input_fd < 0) {
		fprintf(stderr, "Cannot open '%s': %s\n", source,
			strerror(errno));
		return 1;
	}

	size_t block_size = 65536;
	char *block = malloc(block_size);

	if (!block) {
		fprintf(stderr, "Out of memory reading '%s'\n", source);
		close(input_fd);
		return 1;
	}

	int error = 0;
	ssize_t bytes_read;

	while (!error && (bytes_read = read(input_fd, block, block_size)) > 0) {
		ssize_t bytes_written = 0;

		while (!error && bytes_written < bytes_read) {
			ssize_t result = write(output_fd, block + bytes_written,
					       (size_t)(bytes_read -
							bytes_written));
			if (result < 0) {
				fprintf(stderr, "Write error: %s\n",
					strerror(errno));
				error = 1;
			} else {
				bytes_written += result;
			}
		}
	}

	if (bytes_read < 0) {
		fprintf(stderr, "Read error in '%s': %s\n", source,
			strerror(errno));
		error = 1;
	}

	free(block);
	close(input_fd);
	return error;
}

/* Esta funcion recorre recursivamente el directorio y sus subdirectorios
*   ademas copia el contenido de los archivos .txt y evita copiar el
*   archivo de salida.
*/
static int
walk_directory(const char *directory, int output_fd, ino_t output_inode)
{
	DIR *dir = opendir(directory);

	if (!dir) {
		fprintf(stderr, "Cannot open directory '%s': %s\n", directory,
			strerror(errno));
		return 1;
	}

	int error = 0;
	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0
		    || strcmp(entry->d_name, "..") == 0)
			continue;

		char *full_path = build_path(directory, entry->d_name);

		if (!full_path) {
			fprintf(stderr, "Out of memory\n");
			error = 1;
			break;
		}

		struct stat info;

		if (stat(full_path, &info) < 0) {
			fprintf(stderr, "Cannot stat '%s': %s\n", full_path,
				strerror(errno));
			free(full_path);
			error = 1;
			continue;
		}

		if (S_ISDIR(info.st_mode)) {
			if (walk_directory(full_path, output_fd, output_inode)
			    != 0)
				error = 1;
		} else if (S_ISREG(info.st_mode) && ends_with_txt(entry->d_name)
			   && info.st_ino != output_inode) {
			if (copy_file(full_path, output_fd) != 0)
				error = 1;
		}

		free(full_path);
	}

	closedir(dir);
	return error;
}

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr,
			"%s: exactly one argument required (output path)\n",
			argv[0]);
		return 1;
	}
	if (argc > 2) {
		fprintf(stderr,
			"%s: too many arguments; exactly one required\n",
			argv[0]);
		return 1;
	}

	int output_fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);

	if (output_fd < 0) {
		fprintf(stderr, "%s: cannot open '%s': %s\n", argv[0], argv[1],
			strerror(errno));
		return 1;
	}

	struct stat output_info;

	if (fstat(output_fd, &output_info) < 0) {
		fprintf(stderr, "%s: cannot stat '%s': %s\n", argv[0], argv[1],
			strerror(errno));
		close(output_fd);
		return 1;
	}

	int result = walk_directory(".", output_fd, output_info.st_ino);

	close(output_fd);
	return result;
}
