cmake_minimum_required(VERSION 3.0)

project(gexnet-test)


set(test_SOURCES
	test_network.c
)

add_executable(gexnet-test ${test_SOURCES})
target_link_libraries(gexnet-test PUBLIC gexnet)