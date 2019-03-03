cmake_minimum_required(VERSION 3.13)
project(simple)

add_executable(rabbitmq_amqp_handler amqp_handler.cpp)
target_link_libraries(rabbitmq_amqp_handler PRIVATE ${CONAN_LIBS})

add_executable(rabbitmq_tcp_handler tcp_handler.cpp)
target_link_libraries(rabbitmq_tcp_handler PRIVATE ${CONAN_LIBS})

add_executable(rabbitmq_asio_handler asio_handler.cpp)
target_link_libraries(rabbitmq_asio_handler PRIVATE ${CONAN_LIBS})