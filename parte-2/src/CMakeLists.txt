cmake_minimum_required(VERSION 3.23)
project(parte2_astar CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(preprocess
    src/preprocess.cpp
)

add_executable(astar_run
    src/astar_main.cpp
)

target_include_directories(preprocess PRIVATE include)
target_include_directories(astar_run PRIVATE include)

if (NOT MSVC)
  target_compile_options(preprocess PRIVATE -O3 -Wall -Wextra)
  target_compile_options(astar_run PRIVATE -O3 -Wall -Wextra)
endif()
