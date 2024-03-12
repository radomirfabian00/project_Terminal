#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NMAX 100
#define TOK_MAX 30
#define TOO_MANY_ARGUMENTS_ERROR 0x01
#define TOO_FEW_ARGUMENTS_ERROR 0x02
#define CANNOT_ACCESS_ERROR 0x04

int cd(int argc, char **argv);