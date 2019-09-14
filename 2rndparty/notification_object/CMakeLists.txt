project(notification_object)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-pedantic -fPIC -Wall -Wextra -fabi-version=10)

# Library target
add_library(${PROJECT_NAME} INTERFACE)
# target_link_libraries (${PROJECT_NAME} INTERFACE notification)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES "./include/*")
target_sources(${PROJECT_NAME} INTERFACE ${SOURCES})

add_subdirectory(tests)
