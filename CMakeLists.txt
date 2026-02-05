cmake_minimum_required(VERSION 3.10)
project(iotester)
set (CMAKE_CXX_STANDARD 20)

add_executable(iotester main.cpp bufferfiller.cpp bufferqueue.cpp hashedbuffer.cpp filewriter.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Cryptopp REQUIRED IMPORTED_TARGET libcryptopp)
target_link_libraries(iotester PUBLIC PkgConfig::Cryptopp)

install(TARGETS iotester RUNTIME DESTINATION bin)
