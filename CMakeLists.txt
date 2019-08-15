cmake_minimum_required(VERSION 3.10)

project(msplatform VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

option(WITH_TESTS "Enable Unit Tests" OFF)

option(WITH_ASAN "Build with Address Sanitizer" OFF)
option(WITH_TSAN "Build with Threads Sanitizer" OFF)
option(WITH_MSAN "Build with Memory Sanitizer"  OFF)

set(AMP_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

#----------------------------------------------------------
# Platform targets
#----------------------------------------------------------
option(WITH_ROUTER          "Build ROUTER"          ON)
option(WITH_CCLIENT         "Build CCLIENT"         ON)
option(WITH_RCLIENT         "Build RCLIENT"         ON)

if(WITH_TSAN)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fsanitize=thread -g -O2 -fPIC -pie")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -g -O2 -fPIC -pie")
endif()

if(WITH_ASAN)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -g -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
endif()

if(WITH_MSAN)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fsanitize=memory -fno-omit-frame-pointer -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -fno-omit-frame-pointer -g")
endif()

include(ProjectDeps)

include_directories(3rdparty/cppzmq)
include_directories(3rdparty/json/include)

add_subdirectory(3rdparty/proto)

if(WITH_ROUTER)
    add_subdirectory(router)
endif()

if(WITH_CCLIENT)
    add_subdirectory(cclient)
endif()

if(WITH_RCLIENT)
    # Enable ExternalProject CMake module
    include(ExternalProject)

    # Add rclient as a CMake Rust target
    ExternalProject_Add(
            rclient
            DOWNLOAD_COMMAND ""
            CONFIGURE_COMMAND ""
            BUILD_COMMAND cargo build COMMAND cargo build --release
            BINARY_DIR "${CMAKE_SOURCE_DIR}/rclient"
            INSTALL_COMMAND ""
            LOG_BUILD ON)

    set(SERVICE_ZMQ_URL "ipc:///tmp/rclient")
    set(SERVICE_ZMQ_TIMEOUT 5000)
    set(SERVICE_DAEMON_PIDFILE /tmp/rclient.pid)
    set(SERVICE_LOG_PATH /tmp/rclient.log)
    set(SERVICE_LOG_LEVEL INFO)
    set(SERVICE_CHANNEL_TIMEOUT_SECS 1)
    set(SERVICE_CHANNEL_TIMEOUT_NANOS 0)
    set(ROUTER_ZMQ_URL "ipc:///tmp/router")
    configure_file(rclient/etc/rclient.yaml.in ${CMAKE_BINARY_DIR}/rclient.yaml)
endif()

if(WITH_TESTS)
    enable_testing()
endif()
