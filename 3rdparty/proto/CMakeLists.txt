cmake_minimum_required(VERSION 3.10)
project(proto)

include_directories(${PROTOBUF_INCLUDE_DIR})
protobuf_generate_cpp(PROTO_HEADER PROTO_SRC falcon.proto)
add_library(proto ${PROTO_HEADER} ${PROTO_SRC})
target_include_directories(proto PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
target_link_libraries(proto INTERFACE protobuf::libprotobuf)
