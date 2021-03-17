#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    FILE *fps = NULL;
    FILE *fpd = NULL;
    char buf[1024];
    size_t n = 0;

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

    while ((n = fread(buf, 1, 1024, fps)) != 0) {
        fwrite(buf ,1 , n, fpd);
    }


    fclose(fps);
    fclose(fpd);

    exit(0);
}

