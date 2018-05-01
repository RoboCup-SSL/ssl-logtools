include_directories(${PROTOBUF_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

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
target_link_libraries(logplayer common)
if(USE_QT5)
	target_link_libraries(logplayer Qt5::Widgets Qt5::Network)
else()
	target_link_libraries(logplayer ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY})
	include_directories(${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTNETWORK_INCLUDE_DIR})
endif()
