#include "server.hpp"

int run() {
    int initLogRet = initLogger();
    if (initLogRet < 0) {
        return -errno;
    }

    int serverSocketFd = initServer();
    if (serverSocketFd < 0) {
        return -errno;
    }

    int loopRet = loopServer(serverSocketFd);
    if (loopRet < 0) {
        return -errno;
    }

    int shutRet = shutdownServer(serverSocketFd);
    if (shutRet < 0) {
        return -errno;
    }

    int cleanRet = cleanupLogger();
    if (cleanRet < 0) {
        return -errno;
    }

    return 0;
}

int main() {
    return run();
}
