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
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define L_FLAG 0x01
#define A_FLAG 0x02 
#define F_FLAG 0x04
#define S_FLAG 0x08
#define NO_FLAG 0x10

#include "ls.h"

char ls_flags = (char)0x10;

int ls(int argc, char **argv)
{
    int noFiles = 0;
    char *files[NUMBER_OF_FILES];
    int result = 0;
    int has_flags = 0;
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-l"))
        {
            ls_flags |= L_FLAG;
            has_flags = 1;
        }
        else if (strcmp(argv[i], "-a") == 0)
        {
            ls_flags |= A_FLAG;
            has_flags = 1;
        }
        else if (strcmp(argv[i], "-F") == 0)
        {
            ls_flags |= F_FLAG;
            has_flags = 1;
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            ls_flags |= S_FLAG;
            has_flags = 1;
        }
        else
        {
            files[noFiles] = (char *)calloc(strlen(argv[i]), sizeof(char));
            strcpy(files[noFiles], argv[i]);
            ++noFiles;
        }
    }

    if (noFiles == 0)
    {
        result = ls_d(".");
    }
    else
    {
        for (int i = 0; i < noFiles; ++i)
        {
            result = ls_d(files[i]);
        }
    }
}

int ls_d(char *source)
{
    int result;
    struct dirent *de;
    struct stat *buf;
    struct stat sa;
    struct group *group;
    struct passwd *passwd;
    char *time;
    DIR *d;
    char symb[2];
    de = (struct dirent *)malloc(sizeof(struct dirent));

    if ((d = opendir(source)) < 0)
    {
        fprintf(stderr, "Directory %s cannot be accessed\n", source);
        return NOT_FOUND;
    }

    while ((de = readdir(d)) != NULL)
    {
        stat(de->d_name, &sa);

        if (ls_flags & L_FLAG)
        {
            if (ls_flags & S_FLAG)
            {
                printf("%3d ", (int)sa.st_blocks);
            }

            if (S_ISDIR(sa.st_mode))
                printf("d");
            else
                printf("-");
            if (sa.st_mode & S_IRUSR)
                printf("r");
            else
                printf("-");
            if (sa.st_mode & S_IWUSR)
                printf("w");
            else
                printf("-");
            if (sa.st_mode & S_IXUSR)
                printf("x");
            else
                printf("-");
            if (sa.st_mode & S_IRGRP)
                printf("r");
            else
                printf("-");
            if (sa.st_mode & S_IWGRP)
                printf("w");
            else
                printf("-");
            if (sa.st_mode & S_IXGRP)
                printf("x");
            else
                printf("-");
            if (sa.st_mode & S_IROTH)
                printf("r");
            else
                printf("-");
            if (sa.st_mode & S_IWOTH)
                printf("w");
            else
                printf("-");
            if (sa.st_mode & S_IXOTH)
                printf("x");
            else
                printf("-");

            if (de->d_name != 0) {
                if (ls_flags & F_FLAG) {
                    if (S_ISDIR(sa.st_mode)) strcpy(symb, "/");
                    else {
                        if (S_ISREG(sa.st_mode)) strcpy(symb, " ");
                        if (is_exec(de->d_name)) strcpy(symb, "*");
                        if (S_ISLNK(sa.st_mode)) strcpy(symb, "@");
                    }
                } else strcpy(symb, " ");

                stat(de->d_name, &sa);
                time = (char*)ctime(&sa.st_atime);
                time[16] = '\0';
                time += 4;
                passwd = getpwuid(sa.st_uid);
                group = getgrgid(sa.st_gid);
                if (ls_flags & A_FLAG) {
                     printf("%3u %-8s %-7s %9d %s %s%s\n", sa.st_nlink, passwd->pw_name, group->gr_name, sa.st_size, time, de->d_name, symb);
                } else {
                    if(strcmp(de->d_name,".")!=0&&strcmp(de->d_name,"..")!=0)
				        if(de->d_name[0]!='0')
					        printf("%3u %-8s %-7s %9d %s %s%s\n",sa.st_nlink, passwd->pw_name, group->gr_name,sa.st_size,time, de->d_name,symb);
                }
            }
        } else {
            if (ls_flags & S_FLAG) {
                if (ls_flags & A_FLAG)
                    printf("%3d", sa.st_blocks);
                else 
                    if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) 
                        if (de->d_name[0] != '0')
                            printf("%3d", sa.st_blocks);
            }

            if (ls_flags & F_FLAG) {
                if (S_ISDIR(sa.st_mode))
                    strcpy(symb, "/");
                else {
                    if (S_ISDIR(sa.st_mode)) strcpy(symb, "/");
                    else {
                        if (S_ISREG(sa.st_mode)) strcpy(symb, " ");
                        if (is_exec(de->d_name)) strcpy(symb, "*");
                        if (S_ISLNK(sa.st_mode)) strcpy(symb, "@");
                    }
                }
            } else strcpy(symb, " ");

            int i = 0;
            if (ls_flags & A_FLAG) {
                printf("%s%s\n", de->d_name, symb);
                ++i;
            } else {
                if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
                    printf("%s%s\n", de->d_name, symb);
                }
            }
        }
    }

    printf("\n");
}

int is_exec(char* name) {
    if (strrchr(name, ".")) {
        return 0;
    }
    return 1;
}