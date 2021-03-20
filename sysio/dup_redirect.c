#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILE_PATH "/tmp/out"

/* If you cannot change the code below the star line,
* how can you implement a program which will behave like command "echo hello world! > /tmp/out"
*/
int main()
{

    int fd;
    fd = open(FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        perror("open()");
        exit(1);
    }

    dup2(fd, 1);

    if (fd != 1)
        close(fd);
/****************************************************************************/

    fputs("hello world!\n", stdout);
    exit(0);
}
