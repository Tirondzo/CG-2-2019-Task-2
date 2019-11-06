cmake_minimum_required(VERSION 3.5)
project(TinyGLTF)

set(CPP_SOURCES impl.cpp)

include_directories(".")
add_library(TinyGLTF ${CPP_SOURCES})