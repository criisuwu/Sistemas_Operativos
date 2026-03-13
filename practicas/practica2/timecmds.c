/*
*   Ejercicio 2. Timecmds
*
*   El programa esta preparado para recibir un número indeterminado de comandos
*   (cada uno con su ruta y posibles argumentos) y los ejecuta de forma concurrente 
*   creando un proceso hijo por cada comando. Cuando cada proceso haya terminado
*   (failure o success) se muestra por la teminal una linea que indica:
*       -El comando ejecutado
*       -El PID del proceso
*       -El tiempo de ejecucion (en segundos)
*       -El estado de salida (failure o success)
*   
*   Por ejempo:
*       $> timecmds '/bin/sleep 10' '/bin/sleep 10' '/bin/false'
*       cmd: /bin/false, pid: 3434, time: 0 seconds, status: failure
*       cmd: /bin/sleep 10, pid: 3432, time: 10 seconds, status: success
*       cmd: /bin/sleep 10, pid: 3433, time: 10 seconds, status: success
*
*   Creado por: Cristina Homobono Fernández
*   Fecha: 13 marzo 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

/* Función para verificar si el numero de
*   de argumentos es valido.
*/
void
check_arg(int argc)
{
	if (argc < 2) {
		fprintf(stderr, "Error de argumentos\n");
		exit(EXIT_FAILURE);
	}
}

/* Función para verificar que los mallocs
*   se hayan creado correctamente.
*/
void
malloc_check(pid_t *pids, time_t *start_times, char **cmds)
{
	if (!pids) {
		fprintf(stderr, "malloc failed\n");
		free(pids);
		exit(EXIT_FAILURE);
	}
	if (!start_times) {
		fprintf(stderr, "malloc failed\n");
		free(start_times);
		exit(EXIT_FAILURE);
	}
	if (!cmds) {
		fprintf(stderr, "malloc failed\n");
		free(cmds);
		exit(EXIT_FAILURE);
	}
}

/* Función utilizada para dividir el comando
*   en palabras para poder pasarlo por execv
*   despues.
*/
static int
parse_cmd_args(char *cmd_copy, char **args, int max_args)
{
	int num_args = 0;
	char *saveptr;
	char *token = strtok_r(cmd_copy, " ", &saveptr);

	while (token != NULL && num_args < max_args) {
		args[num_args++] = token;
		token = strtok_r(NULL, " ", &saveptr);
	}
	args[num_args] = NULL;
	return num_args;
}

/* Esta función copia el comando y lo pasa por parse_cmd_args
*   para que tenga el formato correcto para poder pasarlo por
*   el comando execv.
*/
static pid_t
f_execmd(char *cmd)
{
	char *cmd_copy = strdup(cmd);

	if (!cmd_copy) {
		fprintf(stderr, "strdup failed\n");
		free(cmd_copy);
		exit(EXIT_FAILURE);
	}

	char *args[256];

	parse_cmd_args(cmd_copy, args, 255);

	pid_t pid = fork();

	if (pid < 0) {
		fprintf(stderr, "fork failed\n");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) {
		execv(args[0], args);
		fprintf(stderr, "execv failed for %s\n", args[0]);
		exit(EXIT_FAILURE);
	}

	free(cmd_copy);
	return pid;
}

/* Esta función es para encontrar cual es el PID
*   del proceso que ya haya terminado.
 */
static int
find_pid_index(pid_t *pids, int num_cmds, pid_t waited_pid)
{
	for (int i = 0; i < num_cmds; i++) {
		if (pids[i] == waited_pid)
			return i;
	}
	return -1;
}

/* Función para imprimir por terminal el resultado final
*   del proceso: con su PID, el comando, el tiempo y
*   el estado.
 */
static void
print_cmd_result(char *cmd, pid_t waited_pid, long elapsed, int success)
{
	char *exit_status_str = "failure";

	if (success)
		exit_status_str = "success";

	printf("cmd: %s, pid: %d, time: %ld seconds, status: %s\n",
	       cmd, (int)waited_pid, elapsed, exit_status_str);
}

/* Funicón para liberar memoria */
void
f_free(pid_t *pids, time_t *start_times, char **cmds)
{
	free(pids);
	free(start_times);
	free(cmds);
}

int
main(int argc, char *argv[])
{
	check_arg(argc);

	int num_cmds = argc - 1;

	pid_t *pids = malloc(num_cmds * sizeof(pid_t));
	time_t *start_times = malloc(num_cmds * sizeof(time_t));
	char **cmds = malloc(num_cmds * sizeof(char *));

	malloc_check(pids, start_times, cmds);

	for (int cmd_i = 0; cmd_i < num_cmds; cmd_i++) {
		cmds[cmd_i] = argv[cmd_i + 1];
		start_times[cmd_i] = time(NULL);
		pids[cmd_i] = f_execmd(cmds[cmd_i]);
	}

	int all_success = 1;

	for (int waited = 0; waited < num_cmds; waited++) {
		int status;
		pid_t waited_pid = wait(&status);

		if (waited_pid < 0) {
			fprintf(stderr, "wait failed\n");
			exit(EXIT_FAILURE);
		}

		time_t end_time = time(NULL);

		int child_idx = find_pid_index(pids, num_cmds, waited_pid);

		if (child_idx < 0)
			continue;

		long elapsed = (long)(end_time - start_times[child_idx]);
		int success = WIFEXITED(status) && WEXITSTATUS(status) == 0;

		if (!success)
			all_success = 0;

		print_cmd_result(cmds[child_idx], waited_pid, elapsed, success);
	}

	f_free(pids, start_times, cmds);

	if (all_success)
		exit(EXIT_SUCCESS);
	exit(EXIT_FAILURE);
}
