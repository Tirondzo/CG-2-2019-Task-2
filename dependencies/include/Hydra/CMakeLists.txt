cmake_minimum_required(VERSION 3.5)
project(Hydra)

if(APPLE)
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D_POSIX_C_SOURCE=200809L")
endif()

set(CPP_SOURCES HydraVSGFExport.cpp)

add_library(Hydra ${CPP_SOURCES})