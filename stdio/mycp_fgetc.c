#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    FILE *fps = NULL;
    FILE *fpd = NULL;
    int c = 0;

    if (argc < 3)
        fprintf(stderr, "Usage:...");

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

    while ((c = fgetc(fps)) != EOF) {
        fputc(c, fpd);
    }

    fclose(fps);
    fclose(fpd);

    exit(0);
}

