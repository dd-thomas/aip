cmake_minimum_required(VERSION 3.10)

project(EnvironmentTest)

set(EnvironmentTest_SRC
    src/main.cpp
)

add_executable(EnvironmentTest
    ${EnvironmentTest_SRC}
)
