cmake_minimum_required(VERSION 3.16)
project(parte2 CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -Wall -Wextra)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp")

add_executable(parte2 ${SOURCES} ${HEADERS})
