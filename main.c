#include "cmd_utils.h"

#define BUFFER_SIZE 256
#define MAX_SPLITS 30

#define PIPE_ERROR 0x01
#define FORK_ERROR 0x02

char *home, *restore;
int fdo, fdw;

int main(int argc, char **argv) {
    char *cmd;

    int i, exit_code, result;
    char *line = (char*) calloc(BUFFER_SIZE, sizeof(char));
    char *buffer = (char*) calloc(BUFFER_SIZE, sizeof(char));
    home = (char*) calloc(BUFFER_SIZE, sizeof(char));
    home = getcwd(buffer, BUFFER_SIZE);
    restore = (char*) calloc(BUFFER_SIZE, sizeof(char));
    pid_t child;
    int stat;
    char *nl, *part;
    do {
        printf("&>");
        fgets(line, BUFFER_SIZE - 1, stdin);
        if (strcmp(line, "quit\n")) {
            nl = strdup(line);
            part = line;
            printf("%s", part);
            if (part != NULL) {
                char *c_restore = (char*) calloc(BUFFER_SIZE, sizeof(char));
                int fd[2];
                cmd = strdup(part);
                if (pipe(fd) < 0) {
                    fprintf(stderr, "Error: Pipe()\n");
                    return PIPE_ERROR;
                }

                if ((child=fork()) < 0) {
                    fprintf(stderr, "Error: fork()\n");
                    return FORK_ERROR;
                }

                if (child == 0) {
                    result = parse_command(cmd);
                    printf("%d", result);
                    write(fd[1], restore, BUFFER_SIZE);
                    exit(1);
                } else {
                    close(fd[1]);
                }
                wait(&stat);
                read(fd[0], c_restore, BUFFER_SIZE);

                if (c_restore != NULL && strcmp(c_restore, "")) {
                    chdir(c_restore);
                }

                close(fd[0]);
                free(c_restore);
            }

            free(nl);
            free(cmd);
        }
    } while (strcmp(line, "quit\n"));

    free(buffer);
    free(line);
    return 0;
}