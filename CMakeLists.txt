cmake_minimum_required(VERSION 2.8.2)
project(ssl-logtools)

find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Qt4 4.6.0 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
find_package(Boost 1.42.0 COMPONENTS program_options REQUIRED)
find_package(ZLIB REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
