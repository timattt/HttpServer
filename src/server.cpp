#include "server.hpp"

int initServer() {
    logMessage("init started");

    // CREATING SOCKET
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        REPORT_ERROR("socket");
        return -errno;
    }

    // SETSOCKOPT
    int yes = 1;
    int setSockRet = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (setSockRet == -1) {
        REPORT_ERROR("setsockopt");
        return -errno;
    }

    // BIND
    struct sockaddr_in host_addr = {0};
    host_addr.sin_family = AF_INET;      // host byte order
    host_addr.sin_port = htons(PORT);    // short, network byte order
    host_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(&(host_addr.sin_zero), '\0', 8); // zero the rest of the struct
    int bindRet = bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr));
    if (bindRet == -1) {
        REPORT_ERROR("binding socket");
        return -errno;
    }

    // LISTEN
    int listRet = listen(sockfd, 20);
    if (listRet == -1) {
        REPORT_ERROR("listening on socket");
        return -errno;
    }

    return sockfd;
}

int loopServer(int serverSockFd) {
    logMessage("Starting loop");
    while(1) {
        // ACCEPT
        socklen_t sin_size = sizeof(struct sockaddr_in);
        struct sockaddr_in client_addr = {0};
        int newSockfd = accept(serverSockFd, (struct sockaddr*) &client_addr, &sin_size);
        if(newSockfd == -1) {
            REPORT_ERROR("accept")
            return errno;
        }

        // HANDLE
        int handleRet = handleConnection(newSockfd, &client_addr);
        if (handleRet < 0) {
            return errno;
        }
    }
    return 0;
}

int shutdownServer(int sockfd) {
    shutdown(sockfd, SHUT_RDWR);
    return 0;
}


