#pragma once

#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

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

int initLogger();
int cleanupLogger();
int logMessage(const char * format, ...);

int initServer();
int loopServer(int socket);
int shutdownServer(int socket);

int handleConnection(int sockfd, struct sockaddr_in *client_addr_ptr);

int getFileSize(int fd);