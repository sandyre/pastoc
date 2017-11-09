cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(pastoc)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCES
	src/main.cpp
	src/app.cpp
	src/parser.cpp
	)

add_executable(pastoc ${SOURCES})
