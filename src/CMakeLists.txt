include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/src)

add_subdirectory(protobuf)
add_subdirectory(qt)
add_subdirectory(common)

add_subdirectory(logconvert)
#add_subdirectory(logdaemon)
add_subdirectory(logplayer)
add_subdirectory(logrecorder)

add_subdirectory(examples)
