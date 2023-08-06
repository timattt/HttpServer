cmake_minimum_required(VERSION 3.0)
project(WebServer)

add_executable(WebServer main.cpp server.cpp server.hpp connection.cpp utils.cpp log.cpp)
