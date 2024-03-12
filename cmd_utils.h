#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define ARGUMENT_NUMBER 16

#ifndef PIPE_ERROR

#define PIPE_ERROR 0x01

#endif

#ifndef FORK_ERROR

#define FORK_ERROR 0x02

#endif

#define PIPE_READ_ERROR 0x04
#define FILE_DESCRIPTOR_DUP_ERROR 0x08
#define FILE_OPEN_ERROR 0x10

int exec_command(int, char**);
int parse_command(char*);