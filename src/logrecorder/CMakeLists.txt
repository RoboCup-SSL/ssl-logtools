include_directories(${Boost_INCLUDE_DIRS})

set(SOURCES
    logrecorder.cpp
)

add_executable(logrecorder ${SOURCES})
target_link_libraries(logrecorder common ${Boost_LIBRARIES})
if(USE_QT5)
	target_link_libraries(logrecorder Qt5::Core Qt5::Network)
else()
	target_link_libraries(logrecorder ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY})
	include_directories(${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTNETWORK_INCLUDE_DIR})
endif()
