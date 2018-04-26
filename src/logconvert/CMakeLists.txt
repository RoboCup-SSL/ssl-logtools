include_directories(${Boost_INCLUDE_DIRS})

set(SOURCES
    logconvert.cpp
)

add_executable(logconvert ${SOURCES})
target_link_libraries(logconvert common ${Boost_LIBRARIES} ${QT_QTCORE_LIBRARY})
include_directories(${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR})

