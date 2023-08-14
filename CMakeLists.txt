cmake_minimum_required(VERSION 3.26)

project (expression-parser)

set(CMAKE_CXX_STANARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})

add_subdirectory(tests)
