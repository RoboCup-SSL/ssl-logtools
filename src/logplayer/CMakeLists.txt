include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(QT_USE_QTGUI true)
set(QT_USE_QTNETWORK true)
include(${QT_USE_FILE})

set(SOURCES
    logplayer.cpp
    mainwindow.cpp
    player.cpp
)

set(MOC_SOURCES
    mainwindow.h
    player.h
)

set(UI_SOURCES
    mainwindow.ui
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_SOURCES})
qt4_wrap_ui(UIC_SOURCES ${UI_SOURCES})

add_executable(logplayer ${SOURCES} ${MOC_SOURCES} ${UIC_SOURCES})
target_link_libraries(logplayer common ${QT_LIBRARIES})
