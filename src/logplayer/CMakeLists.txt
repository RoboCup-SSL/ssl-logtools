include_directories(${PROTOBUF_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    logplayer.cpp
    mainwindow.cpp
    player.cpp
    mainwindow.h
    player.h
    mainwindow.ui
)

add_executable(logplayer ${SOURCES})
target_link_libraries(logplayer common Qt6::Widgets Qt6::Network)