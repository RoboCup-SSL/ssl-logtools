include_directories(${PROTOBUF_INCLUDE_DIR})

set(PROTO_FILES
    messages_robocup_ssl_detection.proto
    messages_robocup_ssl_geometry.proto
    messages_robocup_ssl_geometry_legacy.proto
    ssl_referee.proto
    messages_robocup_ssl_wrapper.proto
    messages_robocup_ssl_wrapper_legacy.proto
)

protobuf_generate_cpp(PROTO_SOURCES PROTO_HEADERS ${PROTO_FILES})

add_library(protobuf ${PROTO_SOURCES} ${PROTO_HEADERS} ${PROTO_FILES})
target_link_libraries(protobuf ${PROTOBUF_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

