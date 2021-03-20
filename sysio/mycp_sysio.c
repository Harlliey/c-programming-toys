#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char *argv[])
{

    if (argc < 3) {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    int sfd, dfd, num, ret, pos;
    char buf[BUFSIZE];

    sfd = open(argv[1], O_RDONLY);
    if (sfd < 0) {
        perror("open()");
        exit(1);
    }

    dfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (dfd < 0) {
        close(sfd);
        perror("open()");
        exit(1);
    }

    while(1) {
        num = read(sfd, buf, BUFSIZE);
        if (num < 0) {
            perror("read()");
            break;
        }

        if (num == 0) {
            break;
        }

        /*
         * There are serveral reason which may result in a partial read.
         * So place a loop here to force the program to read the whole content before exit.
         */
        pos = 0;
        while (num > 0) {
            ret = write(dfd, buf, num);
            if (ret < 0) {
                perror("write()");
                break;
            }
            pos += ret;
            num -= ret;
        }
    }

    close(dfd);
    close(sfd);

    exit(0);
}
