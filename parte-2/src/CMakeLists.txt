cmake_minimum_required(VERSION 3.16)
project(parte2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -Wall -Wextra")

# Encontrar archivos fuente automáticamente
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp")

add_executable(parte2 ${SOURCES} ${HEADERS})

# Hacer parte-2.py ejecutable
configure_file(parte-2.py parte-2.py COPYONLY)
