find_package(Protobuf REQUIRED)
find_program(PROTOC_EXECUTABLE protoc HINTS /usr/bin)
if(NOT PROTOC_EXECUTABLE)
    message(FATAL_ERROR "protoc executable not found")
endif()

include_directories(${PROTOBUF_INCLUDE_DIR})

set(PROTO_FILES
    messages_robocup_ssl_detection.proto
    messages_robocup_ssl_geometry.proto
    messages_robocup_ssl_geometry_legacy.proto
    ssl_referee.proto
    messages_robocup_ssl_wrapper.proto
    messages_robocup_ssl_wrapper_legacy.proto
)

set(PROTO_SOURCES)
set(PROTO_HEADERS)

foreach(PROTO_FILE ${PROTO_FILES})
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)
    set(GENERATED_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.pb.cc)
    set(GENERATED_HEADER ${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.pb.h)

    add_custom_command(
        OUTPUT ${GENERATED_SOURCE} ${GENERATED_HEADER}
        COMMAND ${PROTOC_EXECUTABLE}
        ARGS --cpp_out=${CMAKE_CURRENT_BINARY_DIR} -I${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_FILE}
        DEPENDS ${PROTO_FILE}
    )

    list(APPEND PROTO_SOURCES ${GENERATED_SOURCE})
    list(APPEND PROTO_HEADERS ${GENERATED_HEADER})
endforeach()

add_library(protobuf ${PROTO_SOURCES} ${PROTO_HEADERS})
target_link_libraries(protobuf ${PROTOBUF_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})