cmake_minimum_required(VERSION 3.10)

project(AIP VERSION "1.0")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

if(NOT DEFINED ENV{WORKSDK})
    message(FATAL_ERROR "Environment variable WORKSDK is not set.")
endif()

# Add main projects
add_subdirectory(BasicUT)
add_subdirectory(EnvironmentTest)