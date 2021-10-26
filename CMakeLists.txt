cmake_minimum_required(VERSION 3.20)
project(game24 C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(game24
        main.c
        helpers.c helpers.h
        boolean.h)
