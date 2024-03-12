#include "cd.h"

#ifndef BUFFER_SIZE 

#define BUFFER_SIZE 256

#endif

extern char *home, *restore;

int cd(int argc, char **argv) {
    char *buffer = (char*) calloc(BUFFER_SIZE, sizeof(char));
    char *lastFolder = (char*) calloc(BUFFER_SIZE, sizeof(char));
    char *actualFolder = (char*) calloc(BUFFER_SIZE, sizeof(char));
    char *homeFolderSafe = strrchr(getcwd(buffer, BUFFER_SIZE), '/');

    int stat;

    if (strcmp(getcwd(buffer, BUFFER_SIZE), home) && strcmp(argv[1], "..")) stat = 1;

    lastFolder = strrchr(getcwd(buffer, BUFFER_SIZE), '/');
    if (argc <= 1) {
        fprintf(stderr, "Error: too few arguments");
        return TOO_FEW_ARGUMENTS_ERROR;
    } else if (argc > 2) {
        fprintf(stderr, "Error: too many arguments");
        return TOO_MANY_ARGUMENTS_ERROR;
    } else {
        if (chdir(argv[1])) {
            fprintf(stderr, "Error: Cannot access directory\n");
            return CANNOT_ACCESS_ERROR;
        } else {
            actualFolder = strrchr(getcwd(buffer, BUFFER_SIZE), '/');
            if (strcmp(actualFolder, homeFolderSafe)) {
                if (stat)
                    printf("%s", lastFolder);
                else 
                    printf("%s", actualFolder);
            }
        }
    }
    
    strcpy(restore, argv[1]);
}