include_directories(${PROTOBUF_INCLUDE_DIR})

set(SOURCES
    recorder.cpp

    network.cpp
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

set(MOC_SOURCES
    recorder.h
    network.h
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_SOURCES})

add_library(common ${SOURCES} ${MOC_SOURCES})
target_link_libraries(common protobuf qt)
if(USE_QT5)
	target_link_libraries(common Qt5::Core Qt5::Network)
else()
	target_link_libraries(common ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY})
	include_directories(${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTNETWORK_INCLUDE_DIR})
endif()

if(WIN32)
    target_link_libraries(common wsock32)
endif()
