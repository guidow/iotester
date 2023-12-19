cmake_minimum_required(VERSION 3.0)

project(iotester)

add_executable(iotester main.cpp)

install(TARGETS iotester RUNTIME DESTINATION bin)
