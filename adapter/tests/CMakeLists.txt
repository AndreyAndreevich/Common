cmake_minimum_required(VERSION 3.13)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(adapter adapter_tst.cpp)

target_include_directories(adapter PRIVATE
        ${Boost_INCLUDE_DIR}
        ${CMAKE_ADAPTER_INCLUDE_DIR}
        )

message(STATUS ${CMAKE_ADAPTER_INCLUDE_DIR})

set_target_properties(adapter PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        LINK_LIBRARIES ${Boost_LIBRARIES}
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
        )

add_test(adapter adapter)