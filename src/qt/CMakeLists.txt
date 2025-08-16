set(SOURCES
    multicastsocket.cpp
    multicastsocket.h
    qtiocompressor.cpp
    qtiocompressor.h
)

add_library(qt ${SOURCES})
target_link_libraries(qt Qt6::Core Qt6::Network ${ZLIB_LIBRARIES})

if(WIN32)
    target_link_libraries(qt wsock32)
endif()