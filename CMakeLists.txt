cmake_minimum_required(VERSION 3.10)

project(dragon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(Boost 1.73 COMPONENTS system REQUIRED)

set(SVC_SRC_MAIN_FILES src/main.cpp)

add_executable(${PROJECT_NAME} ${SVC_SRC_MAIN_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC 3rdparty/args)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
