cmake_minimum_required(VERSION 3.10)

project(dragon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)
find_package(cpprestsdk REQUIRED)
find_package(Boost 1.71 COMPONENTS system REQUIRED)

add_subdirectory(proto)

set(SVC_HDR_FILES
        src/phoenix_service/microsvc_controller.hpp
        src/phoenix_service/task_manager.hpp
        src/rest_service/foundation/include/basic_controller.hpp
        src/rest_service/foundation/include/controller.hpp
        src/rest_service/microsvc_controller.hpp
        src/system/sysinfo/cpuinfo.hpp
        src/system/sysinfo/osinfo.hpp
        src/utils/config.hpp
        src/utils/config_st.hpp
        src/utils/interrupt_handler.hpp
        src/utils/logger.hpp
        )

set(SVC_SRC_FILES
        src/phoenix_service/microsvc_controller.cpp
        src/phoenix_service/task_manager.cpp
        src/rest_service/foundation/basic_controller.cpp
        src/rest_service/microsvc_controller.cpp
        src/system/sysinfo/cpuinfo.cpp
        src/system/sysinfo/osinfo.cpp
        src/utils/config.cpp
        src/utils/logger.cpp
        )

set(SVC_SRC_MAIN_FILES src/main.cpp)

include_directories(src)

add_executable(${PROJECT_NAME} ${SVC_SRC_MAIN_FILES} ${SVC_SRC_FILES} ${SVC_HDR_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC 3rdparty/args)
target_include_directories(${PROJECT_NAME} PUBLIC 3rdparty/spdlog/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cpprestsdk::cpprest proto)
