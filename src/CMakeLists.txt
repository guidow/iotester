cmake_minimum_required(VERSION 3.10)

configure_file ("${PROJECT_SOURCE_DIR}/src/version.h.in" "${PROJECT_BINARY_DIR}/src/version.h")

add_executable(iotester main.cpp bufferfiller.cpp bufferqueue.cpp hashedbuffer.cpp filewriter.cpp iotesteroptions.cpp)
target_include_directories(iotester PRIVATE ${PROJECT_BINARY_DIR}/src)
target_link_libraries(iotester PUBLIC PkgConfig::Cryptopp)
