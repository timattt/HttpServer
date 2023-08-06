#include "server.hpp"

FILE * logFileFd = 0;

int initLogger() {
    const std::string logFileName = "httpServerLog.txt";
    logFileFd = fopen(logFileName.c_str(), "w");

    if (logFileFd == nullptr) {
        return -1;
    }
}

int cleanupLogger() {
    if (logFileFd != nullptr) {
        fclose(logFileFd);
        logFileFd = nullptr;
    }
}

int logMessage(const char * format, ...) {
    va_list valist;
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    fprintf(logFileFd, "[%s] ", (char*) timeinfo);
    fprintf(logFileFd, format, valist);
    fprintf(logFileFd, "\n");
    fflush(logFileFd);

    printf( "[%s] ", (char*) timeinfo);
    printf( format, valist);
    printf( "\n");
    fflush(stdout);
}