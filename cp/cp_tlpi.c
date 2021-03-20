#include "../lib/tlpi_hdr.h"
#include <fcntl.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int input_fd, output_fd, open_flags;
    mode_t file_perms;  //文件访问权限，mode_t在本机器上为unsigned int类型
    ssize_t num_read;   //存放读取的字节数，ssize_t在本机器上为long int类型
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s: old-filename new-filename\n", argv[0]);

    input_fd = open(argv[1], O_RDONLY);  //以只读的方式打开文件
    if (input_fd == -1)
        errExit("opening file %s", argv[1]);

    open_flags = O_CREAT | O_WRONLY | O_TRUNC;
    file_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;  //-rw-rw-rw-

    output_fd = open(argv[2], open_flags, file_perms);
    if (output_fd == -1)
        errExit("opening file %s", argv[2]);

    while ((num_read = read(input_fd, buf, BUF_SIZE)) > 0) {
        if (write(output_fd, buf, num_read) != num_read)
            fatal("couldn't write whole buffer");
    }

    if (num_read == -1)
        errExit("read");

    if (close(input_fd) == -1)
        errExit("close input");
    if (close(output_fd) == -1)
        errExit("close output");

    exit(EXIT_SUCCESS);
}