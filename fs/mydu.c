#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <glob.h>

#define BUFSIZE 1024

// exclude the situation which pathname is "." and ".."
// if we don't examine them, the program will trap into an infinite loop
static int path_check(char *pathname)
{
    char *p;

    if ((p = strrchr(pathname, '/')) == NULL) {
        fprintf(stderr, "Not a path\n");
        exit(1);
    }

    if (!strcmp(p+1, ".") || !strcmp(p+1, ".."))
        return 0;

    return 1;
}

static int64_t mydu(const char *pathname)
{
    struct stat statres;
    glob_t globres;
    char pattern[BUFSIZE];
    size_t i;
    int64_t sum;

    //path pattern: /aaa/bbb/ccc...
    if(lstat(pathname, &statres) == -1) {
        perror("lstat()");
        exit(1);
    }

    // handle the file. In this situation, the disk usage is half
    // of the block number, since the block size is 512 byte
    if (!S_ISDIR(statres.st_mode)) {
        return statres.st_blocks;
    }

    // handle the directory recursively
    strncpy(pattern, pathname, BUFSIZE);
    strncat(pattern, "/*", BUFSIZE-1);

    if (glob(pattern, 0, NULL, &globres) != 0) {
        fprintf(stderr, "error: glob()\n");
        exit(1);
    }

    strncpy(pattern, pathname, BUFSIZE);
    //use BUFSIZE-1 to avoid Wstrigop
    strncat(pattern, "/.*", BUFSIZE-1);

    if (glob(pattern, GLOB_APPEND, NULL, &globres) != 0) {
        fprintf(stderr, "error: glob()\n");
        exit(1);
    }

    sum = 0;
    for (i = 0; i < globres.gl_pathc; i++) {
        if (path_check(globres.gl_pathv[i])) 
            sum += mydu(globres.gl_pathv[i]);
    }

    sum += statres.st_blocks;

    // remember to free the memory in the end
    globfree(&globres);
    return sum;
}

int main(int argc, char **argv)
{
    char pathbuf[BUFSIZE];
    int64_t sum;
    int i;

    // if no argument, demonstrate the disk usage of
    // the current working directory
    if (argc == 1) {
        if (getcwd(pathbuf, BUFSIZE) == NULL) {
            perror("getcwd()");
            exit(1);
        }
        sum = mydu(pathbuf);
        fprintf(stdout, "%-8ld%c\n", sum/2, '.');
        exit(0);
    }

    // parsing the arguments one by one
    for (i = 1; i < argc; i++) {
        sum = mydu(argv[i]);
        fprintf(stdout, "%-8ld%s\n", sum/2, argv[i]);
    }

    exit(0);
}
