#include "server.hpp"

int sendString(int sockfd, const std::string & str) {
    int sentBytes = 0;
    int bytesToSend = str.length();
    const char * buffer = str.c_str();
    while(bytesToSend > 0) {
        sentBytes = send(sockfd, buffer, bytesToSend, 0);
        if(sentBytes == -1) {
            REPORT_ERROR("error while sending")
            return -errno;
        }
        bytesToSend -= sentBytes;
        buffer += sentBytes;
    }
    return 0;
}

int recvLine(int sockfd, std::string & result) {
    char c = 0;
    while(recv(sockfd, &c, 1, 0) == 1) {
        if (c == '\r' || c == '\n') {
            return 0;
        }

        result += c;
    }

    REPORT_ERROR("no new line symbols in message");
    return -1;
}

int handleConnection(int sockfd, struct sockaddr_in *client_addr_ptr) {
    // RECEIVE LINE
    std::string request;
    int recvRet = recvLine(sockfd, request);
    if (recvRet < 0) {
        return -errno;
    }

    auto ip = inet_ntoa(client_addr_ptr->sin_addr);
    auto port = ntohs(client_addr_ptr->sin_port);
    logMessage("From %s:%d [%s]", ip, port, request.c_str());

    // remove HTTP
    const std::string httpMes = " HTTP/";
    int headerPos = request.find(httpMes);
    if (headerPos == std::string::npos) {
        logMessage("NOT HTTP");
        shutdown(sockfd, SHUT_RDWR);
        return 0;
    }
    request = request.substr(0, headerPos);

    // extract url from GET and POST
    const std::string getMes = "GET ";
    const std::string headMes = "HEAD ";

    int getPos = request.find(getMes);
    int headPos = request.find(headMes);

    bool isGet = getPos != std::string::npos;
    bool isHead = headPos != std::string::npos;

    int urlPos = 0;

    if (isGet) {
        logMessage("get request");
        urlPos = getPos + getMes.size();
    }
    if (isHead) {
        logMessage("head request");
        urlPos = headPos + headMes.size();
    }

    std::string url = request.substr(urlPos, request.size() - urlPos);

    if (url.empty()) {
        logMessage("empty url");
        shutdown(sockfd, SHUT_RDWR);
        return 0;
    }

    logMessage("received url [%s]", url.c_str());

    // preparing resource path
    std::string resource = url;
    // INDEX.HTML
    if (resource.back() == '/') {
        resource += "index.html";
    }
    resource = WEBROOT + resource;

    // open resource
    int fd = open(resource.c_str(), O_RDONLY, 0);
    if(fd == -1) {
        logMessage("404 Not Found");
        sendString(sockfd, "HTTP/1.0 404 NOT FOUND\r\n");
        sendString(sockfd, "Server: Tiny webserver\r\n\r\n");
        sendString(sockfd, "<html><head><title>404 Not Found</title></head>");
        sendString(sockfd, "<body><h1>gggURL not found</h1></body></html>\r\n");
    } else {
        logMessage("Sending file [%s]\n", resource.c_str());
        sendString(sockfd, "HTTP/1.0 200 OK\r\n");
        sendString(sockfd, "Server: Tiny webserver\r\n\r\n");

        int length = getFileSize(fd);
        if(length < 0) {
            logMessage("getting resource file size error");
            shutdown(sockfd, SHUT_RDWR);
            return -errno;
        }

        char * ptr = (char *) calloc(length, 1);
        if (ptr == NULL) {
            REPORT_ERROR("allocating memory for reading resource");
            shutdown(sockfd, SHUT_RDWR);
            return -errno;
        }

        read(fd, ptr, length);
        send(sockfd, ptr, length, 0);
        free(ptr);
        close(fd);
    }

    shutdown(sockfd, SHUT_RDWR);
    return 0;
}