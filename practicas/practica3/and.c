#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void ft_err(int argc)
{
    if (argc < 3) {
        fprintf(stderr, "ERROR: wrong number of args(./and cmd cmd [cmd...])\n");
        exit(EXIT_FAILURE);
    }
}

int ft_wait(pid_t pid)
{
    int status;

    if (waitpid(pid, &status, 0) < 0) {
        fprintf(stderr, "ERROR: fail to wait for child\n");
        return 1;
    }
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return EXIT_FAILURE;
}

void ft_child(char *path, char **argv)
{
    argv[0] = path;
    execv(path, argv);
    fprintf(stderr, "ERROR: command %s not found\n", path);
    exit(EXIT_FAILURE);
}

int ft_fork(char *path, char **argv)
{
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "ERROR: fail to create child process\n");
        return 1;
    }
    if (pid == 0)
        ft_child(path, argv);
    free(path);
    free(argv);
    return ft_wait(pid);
}

int ft_count_tokens(char *cmd)
{
    int count = 0;
    char *tmp = strdup(cmd);
    if (!tmp) {
        fprintf(stderr, "ERROR: fail to duplicate the command\n");
        return EXIT_FAILURE;
    }
    char *saveptr;
    char *token = strtok_r(tmp, " \t", &saveptr);
    while (token != NULL) {
        count++;
        token = strtok_r(NULL, " \t", &saveptr);
    }
    free(tmp);
    return count;
}

char **ft_tokenize(char *cmd, int *out_argc)
{
    int argc = ft_count_tokens(cmd);
    char **argv = malloc((argc + 1) * sizeof(char *));
    if (!argv) {
        fprintf(stderr, "ERROR: fail to allocate memory\n");
        return NULL;
    }
    int i = 0;
    char *saveptr;
    char *token = strtok_r(cmd, " \t", &saveptr);
    while (token != NULL) {
        argv[i++] = token;
        token = strtok_r(NULL, " \t", &saveptr);
    }
    argv[i] = NULL;
    *out_argc = argc;
    return argv;
}

char *ft_build_path(char *name)
{
    char *prefix = "/bin/";
    char *path = malloc(strlen(prefix) + strlen(name) + 1);
    if (!path) {
        fprintf(stderr, "ERROR: fail to allocate memory\n");
        return NULL;
    }
    strcpy(path, prefix);
    strcat(path, name);
    return path;
}

int run_command(char *cmd)
{
    int argc;
    char **argv = ft_tokenize(cmd, &argc);
    if (!argv)
        return 1;
    if (argc == 0) {
        free(argv);
        return 1;
    }
    char *path = ft_build_path(argv[0]);
    if (!path) {
        free(argv);
        return 1;
    }
    return ft_fork(path, argv);
}

int main(int argc, char *argv[])
{
    int status = 0;

    ft_err(argc);
    for (int i = 1; i < argc; i++) {
        char *cmd = strdup(argv[i]);
        if (!cmd) {
            fprintf(stderr, "ERROR: fail to duplicate the command\n");
            return EXIT_FAILURE;
        }
        status = run_command(cmd);
        free(cmd);
        if (status != 0)
            break;
    }
    return status;
}