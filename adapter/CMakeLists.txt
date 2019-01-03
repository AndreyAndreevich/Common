cmake_minimum_required(VERSION 3.13)
project(adapter)

find_package(Boost REQUIRED)

set(CMAKE_ADAPTER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

message(STATUS ${CMAKE_ADAPTER_INCLUDE_DIR})

add_subdirectory(tests)