#include "../lib/tlpi_hdr.h"
#include <fcntl.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int input_fd, output_fd, open_flags;
    mode_t file_perms;
    ssize_t num_read;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s: old-filename new-filename\n", argv[0]);

    input_fd = open(argv[1], O_RDONLY);
    if (input_fd == -1)
        errExit("opening file %s", argv[1]);

    open_flags = O_CREAT | O_WRONLY | O_TRUNC;
    file_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    output_fd = open(argv[2], open_flags, file_perms);
    if (output_fd == -1)
        errExit("opening file %s", argv[2]);

    while ((num_read = read(input_fd, buf, BUF_SIZE)) > 0) {
        int nonzero_bytes, zero_bytes, cursor, i;
        nonzero_bytes = zero_bytes = cursor = i = 0;

        //拷贝空洞文件的核心逻辑，通过逐字节检查每次读取到buf中的内容寻找空洞字节与非零字节
        while (i < num_read) {
            while (i < num_read && buf[i] != '\0') {
                ++nonzero_bytes;
                ++i;
            }

            if (nonzero_bytes > 0) {
                if (write(output_fd, &buf[cursor], nonzero_bytes) != nonzero_bytes)
                    fatal("couldn't write whole buffer");
            }

            while (i < num_read && buf[i] == '\0') {
                ++zero_bytes;
                ++i;
            }

            if (zero_bytes > 0) {
                if (lseek(output_fd, zero_bytes, SEEK_CUR) == -1)
                    fatal("couldn't write holes");
            }

            cursor = i;
            nonzero_bytes = zero_bytes = 0;
        }
    }

    if (num_read == -1)
        errExit("read");

    if (close(input_fd) == -1)
        errExit("close input");
    if (close(output_fd) == -1)
        errExit("close output");

    exit(EXIT_SUCCESS);
}
