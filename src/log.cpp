#include "server.hpp"

FILE * logFileFd = 0;

int initLogger() {
    const std::string logFileName = "httpServerLog.txt";
    logFileFd = fopen(logFileName.c_str(), "w");

    if (logFileFd == nullptr) {
        return -1;
    }

    return 0;
}

int cleanupLogger() {
    if (logFileFd != nullptr) {
        fclose(logFileFd);
        logFileFd = nullptr;
    }

    return 0;
}

int logMessage(const char * format, ...) {
    va_list valist;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    std::string time = oss.str();

    fprintf(logFileFd, "[%s] ", time.c_str());
    va_start(valist, format);
    vfprintf(logFileFd, format, valist);
    va_end(valist);
    fprintf(logFileFd, "\n");
    fflush(logFileFd);

    printf( "[%s] ", time.c_str());
    va_start(valist, format);
    vprintf( format, valist);
    va_end(valist);
    printf( "\n");
    fflush(stdout);

    return 0;
}