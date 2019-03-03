cmake_minimum_required(VERSION 3.13)

project(beast)

find_package(Threads REQUIRED)

add_executable(asio asio_example.cpp)

set_target_properties(asio PROPERTIES
        COMPILE_DEFINITIONS BOOST_ERROR_CODE_HEADER_ONLY
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
        )

target_compile_definitions(asio PRIVATE
        BOOST_ASIO_ENABLE_HANDLER_TRACKING
        )

target_link_libraries(asio
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

###########

add_executable(async_http_server http_server_async_example.cpp)

set_target_properties(async_http_server PROPERTIES
        COMPILE_DEFINITIONS BOOST_ERROR_CODE_HEADER_ONLY
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
        )

target_compile_definitions(async_http_server PRIVATE
        BOOST_ASIO_ENABLE_HANDLER_TRACKING
        )

target_link_libraries(async_http_server
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )