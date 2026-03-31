cmake_minimum_required(VERSION 3.10)
project(iotester VERSION 0.0.1.0)
set (CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Cryptopp REQUIRED IMPORTED_TARGET libcryptopp)

add_subdirectory(src)

install(TARGETS iotester RUNTIME DESTINATION bin)
