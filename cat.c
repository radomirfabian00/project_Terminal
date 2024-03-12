#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define FILENAME_SIZE 128
#define MAX_FILES 30

#define FAILED_TO_WRITE_STDOUT_ERROR 0x01
#define FAILED_TO_OPEN_FILE_ERROR 0x02

#define B_FLAG 0x01
#define E_FLAG 0x02
#define N_FLAG 0x04
#define S_FLAG 0x08

char flags = 0;
int cat(int argc, char **argv)
{
    char *buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));
    char *files[MAX_FILES];
    int noFiles = 0;

    if (argc == 1)
    {
        ssize_t nread = 0;
        while ((nread = read(STDIN_FILENO, buffer, BUFFER_SIZE)) != 0)
        {
            if (write(STDOUT_FILENO, buffer, nread) != nread)
            {
                fprintf(stderr, "%s\n", "Failed to write to stdout");
                return FAILED_TO_WRITE_STDOUT_ERROR;
            }
        }
    }
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-b") == 0)
        {
            flags |= B_FLAG;
        }
        else if (strcmp(argv[i], "-E") == 0)
        {
            flags |= E_FLAG;
        }
        else if (strcmp(argv[i], "-n") == 0)
        {
            flags |= N_FLAG;
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            flags |= S_FLAG;
        }
        else
        {
            // add file to list
            files[noFiles] = (char *)calloc(FILENAME_SIZE, sizeof(char));
            strcpy(files[noFiles], argv[i]);
            ++noFiles;
        }

        if (flags == 0)
        {
            for (int i = 0; i < noFiles; ++i)
            {
                int fd = open(files[i], O_RDONLY);
                if (fd < 0)
                {
                    fprintf(stderr, "Error opening file %s\n", files[i]);
                    return FAILED_TO_OPEN_FILE_ERROR;
                }
                int nBytes;
                if (flags & S_FLAG) {
                    
                } else
                    while ((nBytes = read(fd, buffer, BUFFER_SIZE)) != 0)
                    {
                        printf("%s", buffer);
                    }
            }
        }
        else if (flags & B_FLAG)
        {
            for (int i = 0; i < noFiles; ++i)
            {
                int fd = open(files[i], O_RDONLY);
                if (fd < 0)
                {
                    fprintf(stderr, "Error opening file %s\n", files[i]);
                    return FAILED_TO_OPEN_FILE_ERROR;
                }
                int nBytes;
                int noRow = 1;
                while ((nBytes = read(fd, buffer, BUFFER_SIZE)) != 0)
                {
                    char *part = strtok(buffer, "\n");
                    while (part != NULL)
                    {
                        if (strcmp(part, "") == 0)
                        {
                            continue;
                        }
                        if (flags & E_FLAG)
                            printf("%d:\t%s$\n", noRow, part);
                        else
                            printf("%d:\t%s\n", noRow, part);
                        ++noRow;
                        part = strtok(NULL, "\n");
                    }
                }
            }
        }
        else if (flags & N_FLAG)
        {
            for (int i = 0; i < noFiles; ++i)
            {
                int fd = open(files[i], O_RDONLY);
                if (fd < 0)
                {
                    fprintf(stderr, "Error opening file %s\n", files[i]);
                    return FAILED_TO_OPEN_FILE_ERROR;
                }
                int nBytes;
                int noRow = 1;
                int wasNl = 0;
                while ((nBytes = read(fd, buffer, BUFFER_SIZE)) != 0)
                {
                    char *part = (char *)calloc(BUFFER_SIZE, sizeof(char));
                    strcpy(part, buffer);
                    char *fin;
                    int restore = 0;
                    while ((fin = strchr(part, '\n')) != NULL)
                    {
                        
                        printf("%d:\t", noRow);
                        ++noRow;
                        for (; part <= fin; ++part)
                        {
                            printf("%c", *part);
                        }
                        if (flags & S_FLAG)
                        {
                            restore = 0;
                            if (*part == '\n') {
                                while (*(part++) == '\n');
                                restore = 1;
                            }
                            if (restore) --part;
                        }
                    }
                    free(part);
                }
            }
        }
    }

    free(buffer);
    return 0;
}