cmake_minimum_required(VERSION 3.15)
project(threads C)

set(CMAKE_C_STANDARD 99)
file(
        GLOB SOURCES
        main.c
        *.c
        *.h
)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(threads ${SOURCES} ${SOURCE_FILES} main.c)