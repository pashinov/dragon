cmake_minimum_required(VERSION 3.10)

project(dragon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(Boost 1.71 COMPONENTS system REQUIRED)

set(SVC_HDR_FILES
        src/sysinfo/cpuinfo.hpp
        src/sysinfo/osinfo.hpp
        src/utils/task_manager.hpp
        )

set(SVC_SRC_FILES
        src/sysinfo/cpuinfo.cpp
        src/sysinfo/osinfo.cpp
        src/utils/task_manager.cpp
        )

set(SVC_SRC_MAIN_FILES src/main.cpp)

include_directories(src)

add_executable(${PROJECT_NAME} ${SVC_SRC_MAIN_FILES} ${SVC_SRC_FILES} ${SVC_HDR_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC 3rdparty/args)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
