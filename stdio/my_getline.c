#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MALLOCSIZE 120

static ssize_t my_getline(char **lineptr, size_t *n, FILE *stream)
{

    int c = 0;
    char *new_ptr = NULL;
    char *cur_pos = NULL;
    size_t count = 0;

    if (lineptr == NULL || n == NULL || stream == NULL) {
        errno = EINVAL;
        perror("my_getline()");
        return -1;
    }


    if (*lineptr == NULL) {
        *n = MALLOCSIZE;
        if ((*lineptr = (char*)malloc(*n)) == NULL) {
            errno = ENOMEM;
            perror("malloc()");
            return -1;
        }
    }

    cur_pos = *lineptr;

    for (;;) {
        c = fgetc(stream);
        if (c == EOF)
            return -1;

        count ++;

        if (count > ((*n) - 1)) {
            *n = (*n) * 2;

            if ((new_ptr = (char*)realloc(*lineptr, *n)) == NULL) {
                errno = ENOMEM;
                perror("realloc()");
                return -1;
            }

            cur_pos = new_ptr + (count - 1);
            *lineptr = new_ptr;
        }

        *cur_pos = (char)c;
        cur_pos ++;

        if (c == '\n')
            break;
    }

    *cur_pos = '\0';
    return (ssize_t)count;
}

int main(int argc, char *argv[])
{

    if (argc < 2) {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    FILE *fp = NULL;
    char *linebuf = NULL;
    size_t linesize = 0;
    ssize_t size = 0;


    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        perror("fopen()");
        exit(1);
    }

    while ((size = my_getline(&linebuf, &linesize, fp)) >= 0) {
        fprintf(stdout, "Line count: %ld\n", size);
        fprintf(stdout, "Line size: %ld\n", linesize);
    }

    free(linebuf);
    fclose(fp);

    exit(0);
}
