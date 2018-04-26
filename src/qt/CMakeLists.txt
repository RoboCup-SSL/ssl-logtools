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
target_link_libraries(qt ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${ZLIB_LIBRARIES})
include_directories(${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTNETWORK_INCLUDE_DIR})

if(WIN32)
    target_link_libraries(core wsock32)
endif()
