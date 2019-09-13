cmake_minimum_required(VERSION 3.10)
project(router)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SVC_HDR_FILES
        ${PROJECT_SOURCE_DIR}/include/service.hpp
        )

set(SVC_SRC_FILES
        ${PROJECT_SOURCE_DIR}/src/service.cpp
        )

set(SVC_SRC_MAIN_FILES
        ${PROJECT_SOURCE_DIR}/src/main.cpp
        )

include_directories(include)
include_directories(${FALCON_BINARY_DIR}/common)

add_executable(${PROJECT_NAME} ${SVC_HDR_FILES} ${SVC_SRC_FILES} ${SVC_SRC_MAIN_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE proto -lzmq)
