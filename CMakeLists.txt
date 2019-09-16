cmake_minimum_required(VERSION 3.10)

project(falcon VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

option(WITH_ASAN "Build with Address Sanitizer" OFF)
option(WITH_TSAN "Build with Threads Sanitizer" OFF)
option(WITH_MSAN "Build with Memory Sanitizer"  OFF)

option(WITH_TESTS "Enable Unit Tests" OFF)

#----------------------------------------------------------
# Platform targets
#----------------------------------------------------------
option(WITH_ROUTER          "Build ROUTER"          ON)
option(WITH_SYSINFO_SVC     "Build SYSINFO-SVC"     ON)

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
include_directories(3rdparty/spdlog/include)
include_directories(3rdparty/json/include)

add_subdirectory(2ndparty)

if(WITH_ROUTER)
    add_subdirectory(router)
endif()

if(WITH_SYSINFO_SVC)
    add_subdirectory(sysinfo-svc)
endif()

if(WITH_TESTS)
    enable_testing()
endif()
