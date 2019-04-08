cmake_minimum_required(VERSION 3.10)

project(msplatform VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

option(WITH_ASAN "Build with Address Sanitizer" OFF)
option(WITH_TSAN "Build with Threads Sanitizer" OFF)
option(WITH_MSAN "Build with Memory Sanitizer" OFF)

set(AMP_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

#----------------------------------------------------------
# Platform targets
#----------------------------------------------------------
option(WITH_BROKER          "Build BROKER"          ON)
option(WITH_CCLIENT         "Build CCLIENT"         ON)

if(WITH_TSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -g -O2 -fPIC -pie")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread -g -O2 -fPIC -pie")
endif()

if(WITH_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
endif()

if(WITH_MSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -fno-omit-frame-pointer -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=memory -fno-omit-frame-pointer -g")
endif()

include(ProjectDeps)

include_directories(common/include)

include_directories(3rdparty/cppzmq)
include_directories(3rdparty/json/include)

add_subdirectory(common)

if(WITH_BROKER)
    add_subdirectory(broker)
endif()

if(WITH_CCLIENT)
    add_subdirectory(cclient)
endif()
