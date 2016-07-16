include_directories(${Boost_INCLUDE_DIRS})
set(QT_DONT_USE_QTGUI true)
include(${QT_USE_FILE})

set(SOURCES
    logconvert.cpp
)

add_executable(logconvert ${SOURCES})
target_link_libraries(logconvert common ${Boost_LIBRARIES} ${QT_LIBRARIES})
