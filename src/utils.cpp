#include "server.hpp"

int getFileSize(int fd) {
    struct stat stat_struct = { 0 };
    if (fstat(fd, &stat_struct) == -1) {
        return -1;
    }
    return (int) stat_struct.st_size;
}