#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Error de argumentos\n");
		exit(EXIT_FAILURE);
	}

	int n = argc - 1;

	pid_t *pids = malloc(n * sizeof(pid_t));

	if (!pids) {
		fprintf(stderr, "malloc failed\n");
		free(pids);
		exit(EXIT_FAILURE);
	}
	time_t *start_times = malloc(n * sizeof(time_t));

	if (!start_times) {
		fprintf(stderr, "malloc failed\n");
		free(start_times);
		exit(EXIT_FAILURE);
	}
	char **cmds = malloc(n * sizeof(char *));

	if (!cmds) {
		fprintf(stderr, "malloc failed\n");
		free(cmds);
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < n; i++) {
		cmds[i] = argv[i + 1];

		char *buf = strdup(cmds[i]);

		if (!buf) {
			fprintf(stderr, "strdup failed\n");
			free(buf);
			exit(EXIT_FAILURE);
		}

		char *args[256];
		int argc_cmd = 0;
		char *saveptr;
		char *token = strtok_r(buf, " ", &saveptr);

		while (token != NULL && argc_cmd < 255) {
			args[argc_cmd++] = token;
			token = strtok_r(NULL, " ", &saveptr);
		}
		args[argc_cmd] = NULL;

		start_times[i] = time(NULL);
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

		pids[i] = pid;
		free(buf);
	}

	int all_success = 1;

	for (int waited = 0; waited < n; waited++) {
		int status;
		pid_t finished_pid = wait(&status);

		if (finished_pid < 0) {
			fprintf(stderr, "wait failed\n");
			exit(EXIT_FAILURE);
		}

		time_t end_time = time(NULL);

		int idx = -1;

		for (int i = 0; i < n; i++) {
			if (pids[i] == finished_pid) {
				idx = i;
				break;
			}
		}

		if (idx < 0)
			continue;

		long elapsed = (long)(end_time - start_times[idx]);
		int success = WIFEXITED(status) && WEXITSTATUS(status) == 0;

		if (!success)
			all_success = 0;

		char *status_str = "failure";

		if (success)
			status_str = "success";

		printf("cmd: %s, pid: %d, time: %ld seconds, status: %s\n",
		       cmds[idx], (int)finished_pid, elapsed, status_str);
	}

	free(pids);
	free(start_times);
	free(cmds);

	if (all_success)
		exit(EXIT_SUCCESS);
	exit(EXIT_FAILURE);
}
