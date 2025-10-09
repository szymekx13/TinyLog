cmake_minimum_required(VERSION 3.10)
project(tinylog)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_library(tinylog src/tinylog.cpp)
add_executable(example examples/example.cpp)
target_link_libraries(example tinylog)