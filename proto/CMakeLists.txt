cmake_minimum_required(VERSION 3.10)
project(proto)

include_directories(${PROTOBUF_INCLUDE_DIR})

protobuf_generate_cpp(LION_PROTO_HEADER LION_PROTO_SRC lion.proto)
add_library(lion_proto ${LION_PROTO_HEADER} ${LION_PROTO_SRC})
target_include_directories(lion_proto PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
target_link_libraries(lion_proto INTERFACE protobuf::libprotobuf)
