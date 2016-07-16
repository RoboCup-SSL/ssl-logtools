cmake_minimum_required(VERSION 2.8.2)
project(ssl-logtools)

find_package(Threads)
find_package(Protobuf)
find_package(Qt4 4.6.0)
find_package(Boost 1.42.0 COMPONENTS program_options)
find_package(ZLIB)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
