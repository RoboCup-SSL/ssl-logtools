cmake_minimum_required(VERSION 3.16)
project(ssl-logtools)

find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets Network REQUIRED)
find_package(Boost 1.42.0 COMPONENTS program_options REQUIRED)
find_package(ZLIB REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)