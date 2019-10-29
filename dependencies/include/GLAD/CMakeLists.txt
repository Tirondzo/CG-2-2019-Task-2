cmake_minimum_required(VERSION 3.5)
project(GLAD)

set(CPP_SOURCES glad.c glad/glad.h)

include_directories(".")
add_library(GLAD ${CPP_SOURCES})