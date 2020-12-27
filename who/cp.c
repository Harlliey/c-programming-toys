//
// Created by Harrywei on 2020/12/25.
//
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void err_handler(char*, char*);

int main(int argc, char **argv)
{
    int fd_in, fd_out, n_chars;
    char buf[BUFFERSIZE];

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s source destination", *argv);
        exit(1);
    }

    if ((fd_in = open(*(argv + 1), O_RDONLY)) == -1)
    {
        err_handler("Cannot open", *(argv + 1));
    }

    if ((fd_out = creat(*(argv + 2), COPYMODE)) == -1)
    {
        err_handler("Cannot create", *(argv + 2));
    }

    while ((n_chars = read(fd_in, buf, BUFFERSIZE)) > 0)
    {
        if (write(fd_out, buf, n_chars) != n_chars)
        {
            err_handler("Write error to", *(argv + 2));
        }

        if (n_chars == -1)
        {
            err_handler("Read error from", *(argv + 1));
        }
    }

    if (close(fd_in) == -1 || close(fd_out) == -1)
    {
        err_handler("Error closing files", "");
    }
}

void err_handler(char *s1, char *s2)
{
    fprintf(stderr, "Error: %s ", s1);
    perror(s2);
    exit(1);
}