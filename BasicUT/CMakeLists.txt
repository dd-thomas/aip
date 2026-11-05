cmake_minimum_required(VERSION 3.10)

project(BasicUT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

set(BasicUT_SRCS
    src/main.cpp
    src/Converter.cpp
    src/Converter.h
)

add_executable(BasicUT
	${BasicUT_SRCS}
)

# Tests
set(BasicUT_TESTS_SRCS
    tests/testMain.cpp
    src/Converter.cpp
)


add_executable(BasicUTRunner 
    ${BasicUT_TESTS_SRCS}
)
