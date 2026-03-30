cmake_minimum_required(VERSION 3.10)

add_executable(iotester main.cpp bufferfiller.cpp bufferqueue.cpp hashedbuffer.cpp filewriter.cpp iotesteroptions.cpp)
target_link_libraries(iotester PUBLIC PkgConfig::Cryptopp)
