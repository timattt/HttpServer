#include "server.hpp"
/*
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

    bool r = 0;
    bool n = 0;
    char c = 0;
    while(recv(sockfd, &c, 1, 0) == 1) {
        if (c == '\r') {
            r = 1;
        } else
        if (c == '\n') {
            n = 1;
        } else {
            r = n = 0;
        }

        if (r && n) {
            result = result.substr(0, result.size() - 1);
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

    logMessage("From %s:%d [%s]", inet_ntoa(client_addr_ptr->sin_addr), ntohs(client_addr_ptr->sin_port), request);

    // HTTP
    int headerPos = request.find(" HTTP/");
    if (headerPos == std::string::npos) {
        logMessage("NOT HTTP");
        shutdown(sockfd, SHUT_RDWR);
        return 0;
    }
    request = request.substr(0, headerPos);

    // GET
    int getPos = request.find("GET ");


    // HEAD
    int headPos = request.find("HEAD ");


    // EMPTY
    if (request.empty()) {
        logMessage("empty");
        shutdown(sockfd, SHUT_RDWR);
        return 0;
    }

    // INDEX.HTML
    if (request.back() == '/') {
        request += "index.html";
    }

    // OPEN RESOURCE
    std::string resource = WEBROOT + request;
    int fd = open(resource.c_str(), O_RDONLY, 0);
    if(fd == -1) { // if file is not found
        logMessage(" 404 Not Found");
        sendString(sockfd, "HTTP/1.0 404 NOT FOUND\r\n");
        sendString(sockfd, "Server: Tiny webserver\r\n\r\n");
        sendString(sockfd, "<html><head><title>404 Not Found</title></head>");
        sendString(sockfd, "<body><h1>gggURL not found</h1></body></html>\r\n");
    } else {
        logMessage("Sending file [%s]\n", resource);
        sendString(sockfd, "HTTP/1.0 200 OK\r\n");
        sendString(sockfd, "Server: Tiny webserver\r\n\r\n");

        int length = getFileSize(fd);
        if(length < 0) {
            logMessage("getting resource file size error");
            shutdown(sockfd, SHUT_RDWR);
            return -errno;
        }

        char * ptr = (char *) calloc(length, 1);
        if(ptr == NULL) {
            logMessage("allocating memory for reading resource");
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
}*/
/*
int send_string(int sockfd, char *buffer) {
    int sent_bytes, bytes_to_send;
    bytes_to_send = strlen(buffer);
    while(bytes_to_send > 0) {
        sent_bytes = send(sockfd, buffer, bytes_to_send, 0);
        if(sent_bytes == -1)
            return 0; // return 0 on send error
        bytes_to_send -= sent_bytes;
        buffer += sent_bytes;
    }
    return 1; // return 1 on success
}

int recv_line(int sockfd, char *dest_buffer) {
#define EOL "\r\n" // End-Of-Line byte sequence
#define EOL_SIZE 2
    char *ptr;
    int eol_matched = 0;

    ptr = dest_buffer;
    while(recv(sockfd, ptr, 1, 0) == 1) { // read a single byte
        if(*ptr == EOL[eol_matched]) { // does this byte match terminator
            eol_matched++;
            if(eol_matched == EOL_SIZE) { // if all bytes match terminator,
                *(ptr+1-EOL_SIZE) = '\0'; // terminate the string
                return strlen(dest_buffer); // return bytes recevied
            }
        } else {
            eol_matched = 0;
        }
        ptr++; // increment the pointer to the next byter;
    }
    return 0; // didn't find the end of line characters
}


int get_file_size(int fd) {
    struct stat stat_struct;

    if(fstat(fd, &stat_struct) == -1)
        return -1;
    return (int) stat_struct.st_size;
}

int handleConnection(int sockfd, struct sockaddr_in *client_addr_ptr) {
    char *ptr, request[500], resource[500];
    int fd, length;

    length = recv_line(sockfd, request);

    printf("Got request from %s:%d \"%s\"\n", inet_ntoa(client_addr_ptr->sin_addr), ntohs(client_addr_ptr->sin_port), request);

    ptr = strstr(request, " HTTP/"); // search for valid looking request
    if(ptr == NULL) { // then this isn't valid HTTP
        printf(" NOT HTTP!\n");
    } else {
        *ptr = 0; // terminate the buffer at the end of the URL
        ptr = NULL; // set ptr to NULL (used to flag for an invalid request)
        if(strncmp(request, "GET ", 4) == 0)  // get request
            ptr = request+4; // ptr is the URL
        if(strncmp(request, "HEAD ", 5) == 0) // head request
            ptr = request+5; // ptr is the URL

        if(ptr == NULL) { // then this is not a recognized request
            printf("\tUNKNOWN REQUEST!\n");
        } else { // valid request, with ptr pointing to the resource name
            if (ptr[strlen(ptr) - 1] == '/')  // for resources ending with '/'
                strcat(ptr, "index.html");     // add 'index.html' to the end
            strcpy(resource, WEBROOT.c_str());     // begin resource with web root path
            strcat(resource, ptr);         //  and join it with resource path
            fd = open(resource, O_RDONLY, 0); // try to open the file
            printf("\tOpening \'%s\'\t", resource);
            if(fd == -1) { // if file is not found
                printf(" 404 Not Found\n");
                send_string(sockfd, "HTTP/1.0 404 NOT FOUND\r\n");
                send_string(sockfd, "Server: Tiny webserver\r\n\r\n");
                send_string(sockfd, "<html><head><title>404 Not Found</title></head>");
                send_string(sockfd, "<body><h1>URL not found</h1></body></html>\r\n");
            } else {      // otherwise, serve up the file
                printf(" 200 OK\n");
                send_string(sockfd, "HTTP/1.0 200 OK\r\n");
                send_string(sockfd, "Server: Tiny webserver\r\n\r\n");
                if(ptr == request + 4) { // then this is a GET request
                    if( (length = get_file_size(fd)) == -1)
                        REPORT_ERROR("getting resource file size");
                    if( (ptr = (char *) malloc(length)) == NULL)
                        REPORT_ERROR("allocating memory for reading resource");
                    read(fd, ptr, length); // read the file into memory
                    send(sockfd, ptr, length, 0);  // send it to socket
                    free(ptr); // free file memory
                }
                close(fd); // close the file
            } // end if block for file found/not found
        } // end if block for valid request
    } // end if block for valid HTTP
    shutdown(sockfd, SHUT_RDWR); // close the socket gracefully
    return 0;
}

*/