set(SOURCES
    multicastsocket.cpp
    multicastsocket.h

    qtiocompressor.cpp
)

set(MOC_SOURCES
    qtiocompressor.h
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_SOURCES})

add_library(qt ${SOURCES} ${MOC_SOURCES})
target_link_libraries(qt ${ZLIB_LIBRARIES})
if(USE_QT5)
	target_link_libraries(qt Qt5::Core Qt5::Network)
else()
	target_link_libraries(qt ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY})
	include_directories(${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTNETWORK_INCLUDE_DIR})
endif()

if(WIN32)
    target_link_libraries(qt wsock32)
endif()
