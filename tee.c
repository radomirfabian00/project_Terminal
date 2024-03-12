#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define OUTPUT_FILE_REQUIRED_ERROR 0x01
#define FAILED_TO_CREATE_OUTPUT_ERROR 0x02
#define FAILED_TO_WRITE_STDOUT_ERROR 0x04
#define FAILED_TO_WRITE_FILE_ERROR 0x08

#define A_FLAG 0x01
#define NO_FLAG 0x02

#include "tee.h"

char tee_flags = 0;

int tee(int argc, char **argv) {
    if (strcmp(argv[1], "-a") == 0) {
        tee_flags |= A_FLAG;
    }

    if ((argc != 2 && tee_flags & A_FLAG == 0) || (argc != 3 && tee_flags & A_FLAG)) {
        fprintf(stderr, "%s\n", "Output file name required");
        return OUTPUT_FILE_REQUIRED_ERROR;
    }
    char *buffer = (char*) calloc(1024, sizeof(char));
    int outfd = -1;
    if (tee_flags & A_FLAG) {
        outfd = open(argv[2], O_CREAT | O_WRONLY | O_APPEND, 0644);
    } else {
        outfd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    }
    if (outfd == -1) {
        fprintf(stderr, "%s\n", "Failed to create output file");
        return FAILED_TO_CREATE_OUTPUT_ERROR;
    }
    ssize_t nread = 0;
    while((nread = read(STDIN_FILENO, buffer, 1024)) != 0) {
        if (write(STDOUT_FILENO, buffer, nread) != nread) {
            fprintf(stderr, "%s\n", "Failed to write to stdout");
            return FAILED_TO_WRITE_STDOUT_ERROR;
        }
        if (write(outfd, buffer, nread) != nread) {
            fprintf(stderr, "%s\n", "Failed to write to file");
            return FAILED_TO_WRITE_FILE_ERROR;
        }
    }
    return 0;

}
