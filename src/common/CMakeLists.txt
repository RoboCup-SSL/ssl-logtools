include_directories(${PROTOBUF_INCLUDE_DIR})

set(SOURCES
    recorder.cpp
    recorder.h

    network.cpp
    network.h
    timer.cpp
    timer.h

    logfile.cpp
    logfile.h

    format/message_type.h
    format/file_format.h
    format/file_format_legacy.cpp
    format/file_format_legacy.h
    format/file_format_timestamp_type_size_raw_message.cpp
    format/file_format_timestamp_type_size_raw_message.h
)

add_library(common ${SOURCES})
target_link_libraries(common protobuf qt Qt6::Core Qt6::Network)

if(WIN32)
    target_link_libraries(common wsock32)
endif()