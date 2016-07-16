include_directories(${Boost_INCLUDE_DIRS})
set(QT_DONT_USE_QTGUI true)
set(QT_USE_QTNETWORK true)
include(${QT_USE_FILE})

set(SOURCES
    logrecorder.cpp
)

add_executable(logrecorder ${SOURCES})
target_link_libraries(logrecorder common ${Boost_LIBRARIES} ${QT_LIBRARIES})
