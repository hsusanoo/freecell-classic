# cmake_minimum_required(VERSION <specify CMake version here>)
project(freecell C)

set(CMAKE_C_STANDARD 11)

add_executable(freecell main.c utils.c utils.h freecell.c freecell.h)