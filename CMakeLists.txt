cmake_minimum_required(VERSION 3.0)

project(gexnet)

# Source files to be used in the library
set(library_SOURCES
    src/network.c
    src/memory.c
)

add_library(gexnet STATIC ${library_SOURCES})

target_include_directories(gexnet
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)