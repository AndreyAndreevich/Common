cmake_minimum_required(VERSION 3.13)
project(examples)

add_executable(rabbitmq_connect_example connect.cpp)
target_link_libraries(rabbitmq_connect_example PRIVATE ${CONAN_LIBS})

add_executable(rabbitmq_distribution_example distribution.cpp)
target_link_libraries(rabbitmq_distribution_example PRIVATE ${CONAN_LIBS})

add_executable(rabbitmq_publish_subscribe_example publish_subscribe.cpp)
target_link_libraries(rabbitmq_publish_subscribe_example PRIVATE ${CONAN_LIBS})

add_executable(rabbitmq_selective_example selective.cpp)
target_link_libraries(rabbitmq_selective_example PRIVATE ${CONAN_LIBS})