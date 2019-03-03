cmake_minimum_required(VERSION 3.13)

project(common)

include(conanbuildinfo.cmake)
conan_basic_setup()
set(CMAKE_SKIP_RPATH OFF)

find_package(Boost REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(adapter)
add_subdirectory(beast)
add_subdirectory(rabbitmq)
add_subdirectory(thread_safe)

enable_testing()