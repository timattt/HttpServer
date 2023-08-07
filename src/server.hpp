#pragma once

#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <iomanip>
#include <ctime>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <vector>

#define REPORT_ERROR(message) logMessage("ERROR WITH MESSAGE {%s} in file {%s} on line {%d}", message, __FILE__, __LINE__);
#define ASSERT(EXPR) if (!(EXPR)) {REPORT_ERROR("assertion failed");}

const int PORT = 80;
const std::string WEBROOT = "./../webroot";

// LOGGER
int initLogger();
int cleanupLogger();
int logMessage(const char * format, ...);

// SERVER
int initServer();
int loopServer(int socket);
int shutdownServer(int socket);

// HTTP
int handleConnection(int sockfd, struct sockaddr_in *client_addr_ptr);

// UTILS
int getFileSize(int fd);