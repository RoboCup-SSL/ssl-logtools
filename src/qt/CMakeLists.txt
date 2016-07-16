set(QT_DONT_USE_QTGUI true)
set(QT_USE_QTNETWORK true)
include(${QT_USE_FILE})

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
target_link_libraries(qt ${QT_LIBRARIES} ${ZLIB_LIBRARIES})

if(WIN32)
    target_link_libraries(core wsock32)
endif()
