#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{

    if (argc < 2) {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    FILE *fp = NULL;
    char *linebuf = NULL;
    size_t linesize = 0;


    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        perror("fopen()");
        exit(1);
    }

    while (getline(&linebuf, &linesize, fp) >= 0) {
        fprintf(stdout, "Line count: %ld\n", strlen(linebuf));
        fprintf(stdout, "Line size: %ld\n", linesize);
    }

    free(linebuf);
    fclose(fp);

    exit(0);
}
