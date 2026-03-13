#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

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

static pid_t
spawn_child(char *cmd)
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

static int
find_pid_index(pid_t *pids, int num_cmds, pid_t waited_pid)
{
	for (int i = 0; i < num_cmds; i++) {
		if (pids[i] == waited_pid)
			return i;
	}
	return -1;
}

static void
print_cmd_result(char *cmd, pid_t waited_pid, long elapsed, int success)
{
	char *exit_status_str = "failure";

	if (success)
		exit_status_str = "success";

	printf("cmd: %s, pid: %d, time: %ld seconds, status: %s\n",
	       cmd, (int)waited_pid, elapsed, exit_status_str);
}

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Error de argumentos\n");
		exit(EXIT_FAILURE);
	}

	int num_cmds = argc - 1;

	pid_t *pids = malloc(num_cmds * sizeof(pid_t));

	if (!pids) {
		fprintf(stderr, "malloc failed\n");
		free(pids);
		exit(EXIT_FAILURE);
	}
	time_t *start_times = malloc(num_cmds * sizeof(time_t));

	if (!start_times) {
		fprintf(stderr, "malloc failed\n");
		free(start_times);
		exit(EXIT_FAILURE);
	}
	char **cmds = malloc(num_cmds * sizeof(char *));

	if (!cmds) {
		fprintf(stderr, "malloc failed\n");
		free(cmds);
		exit(EXIT_FAILURE);
	}

	for (int cmd_i = 0; cmd_i < num_cmds; cmd_i++) {
		cmds[cmd_i] = argv[cmd_i + 1];
		start_times[cmd_i] = time(NULL);
		pids[cmd_i] = spawn_child(cmds[cmd_i]);
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

	free(pids);
	free(start_times);
	free(cmds);

	if (all_success)
		exit(EXIT_SUCCESS);
	exit(EXIT_FAILURE);
}
