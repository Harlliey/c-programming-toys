#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 1024

int main(int argc, char *argv[])
{

    if (argc < 3) {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    int fd1, fd2, num, i;
    char buf[BUFSIZE];
    size_t linebrks = 0;
    int prevbrk = 0, curbrk = 0, pos = 0;
    int rlen, wlen, total_len;
    size_t lineno = atoi(argv[2]);

    fd1 = open(argv[1], O_RDONLY);
    fd2 = open(argv[1], O_RDWR);

    // Locate the line break right before the current line and the line break of current line
    while (1) {
        num = read(fd1, buf, BUFSIZE);
        if (num < 0) {
            perror("read()");
            break;
        }

        if (num == 0) {
            break;
        }

        for (i = 0; i < num; i++) {
            if (buf[i] == '\n')
                linebrks ++;

            if (linebrks == (lineno-1) && prevbrk == 0) {
                prevbrk = pos + i;
            }

            if (linebrks == lineno && curbrk == 0) {
                curbrk = pos + i;
                break;
            }
        }

        if (linebrks == lineno)
            break;

        pos += num;
    }

    // Handle the insufficient line issue
    if (linebrks < lineno) {
        fprintf(stdout, "No Line %ld\n", lineno);
        close(fd1);
        close(fd2);
        exit(0);
    }

    // Set the cursor
    if (lseek(fd1, curbrk+1, SEEK_SET) < 0) {
        perror("lseek()");
        exit(1);
    }


    if (lseek(fd2, prevbrk+1, SEEK_SET) < 0) {
        perror("lseek()");
        exit(1);
    }

    // Clear the buffer
    memset(buf, 0, BUFSIZE);

    // Modify the content by shared file method
    while(1) {
        rlen = read(fd1, buf, BUFSIZE);
        if (rlen < 0) {
            perror("read()");
            exit(1);
        }

        if (rlen == 0) {
            break;
        }

        wlen = write(fd2, buf, rlen);
        if (wlen < 0) {
            perror("write()");
            exit(1);
        }
    }

    // Calculate the total length of the file
    if ((total_len = lseek(fd1, 0, SEEK_END)) < 0) {
        perror("lseek()");
        exit(1);
    }

    // Truncate the file to the right size
    if (ftruncate(fd2, total_len-(curbrk-prevbrk))) {
        perror("ftruncate()");
        exit(1);
    }

    close(fd1);
    close(fd2);

    exit(0);
}
