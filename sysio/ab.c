#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Run and see if the result is different from what you think.
 * Try the command strace to figure out why.
 */

int main()
{

    fputc('a', stdout);
    write(1, "b", 1);

    fputc('a', stdout);
    write(1, "b", 1);

    fputc('a', stdout);
    write(1, "b", 1);

    fputc('\n', stdout);

    exit(0);
}
