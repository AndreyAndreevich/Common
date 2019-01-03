cmake_minimum_required(VERSION 3.13)
project(adapter)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
set(CMAKE_SKIP_RPATH OFF)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(adapter)

enable_testing()