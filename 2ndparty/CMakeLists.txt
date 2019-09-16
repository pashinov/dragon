cmake_minimum_required(VERSION 3.10)
project(2ndparty)

include_directories(include)

#
# Proto
#
add_subdirectory(proto)

#
# Common - Falcon Loging Library
#
set(LOGGING_HDR_FILES include/logger/logger.hpp)
set(LOGGING_SRC_FILES src/logger/logger.cpp)

add_library(falcon-log ${LOGGING_HDR_FILES} ${LOGGING_SRC_FILES})
