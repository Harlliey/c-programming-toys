#include <stdio.h>
#include <stdlib.h>
#define BUFSIZE 1024

int main(int argc, char *argv[])
{

    FILE *fps = NULL;
    FILE *fpd = NULL;
    char buf[BUFSIZE];

    if (argc < 3)
        fprintf(stderr, "Usage:...\n");

    fps = fopen(argv[1], "r");

    if (fps == NULL) {
        perror("fopen()");
        exit(1);
    }

    fpd = fopen(argv[2], "w");

    if (fpd == NULL) {
        perror("fopen()");
        exit(1);
    }

    while (fgets(buf, BUFSIZE, fps) != NULL) {
        fputs(buf, fpd);
    }

    fclose(fps);
    fclose(fpd);

    exit(0);
}

