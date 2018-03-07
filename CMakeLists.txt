cmake_minimum_required(VERSION 3.0)


project(gexnet)


# Source files to be used in the library
set(library_HEADERS
	include/types.h
	include/feedforward.h
	include/memory.h
	include/network.h
	include/processors.h
)
set(library_SOURCES
	src/feedforward.c
    src/network.c
    src/memory.c
	src/processors.c
)

add_library(gexnet STATIC ${library_SOURCES} ${library_HEADERS})

target_include_directories(gexnet
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(test)
