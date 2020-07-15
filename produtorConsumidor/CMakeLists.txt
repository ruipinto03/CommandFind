cmake_minimum_required(VERSION 3.16)
project(produtorConsumidor C)

file(
        GLOB SOURCES
        main.c
        *.c
        *.h
)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_C_STANDARD 99)

add_executable(produtorConsumidor main.c)