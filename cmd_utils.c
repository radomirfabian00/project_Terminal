#include "cmd_utils.h"

#include "ls.h"
#include "cd.h"
#include "tee.h"
#include "cat.h"

int parse_command(char* line) {
    int status = 0;
    int argc = 0;
    char *argv[ARGUMENT_NUMBER];
    char *part;

    part = strtok(line, " \n\t\r");
    while (part != NULL) {
        argv[argc++] = strdup(part);
        part = strtok(NULL, " \n\t\r");
    }

    status = exec_command(argc, argv);
    return status;
}

int exec_command(int argc, char** argv) {
    int status = 2;
    char *cmd = strdup(argv[0]);

    if (argc >= 2) {
        if (strcmp(argv[1], "--help") == 0) {
            // print help
            free(cmd);
            return 0;
        }
    }

    if (strcmp(cmd, "ls") == 0) status = ls(argc, argv);
    else if (strcmp(cmd, "cd") == 0) status = cd(argc, argv);
    else if (strcmp(cmd, "tee") == 0) status = tee(argc, argv);
    else if (strcmp(cmd, "cat") == 0) status = cat(argc, argv);
    else {
        fprintf(stderr, "Command not found!");
    }

    printf("Status: %d\n", status);

    free(cmd);
    return status;
}