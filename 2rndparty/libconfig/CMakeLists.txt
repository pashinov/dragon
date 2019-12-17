project(libconfig)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-pedantic -fPIC -Wall -Wextra -fabi-version=10)

file(GLOB_RECURSE HEADERS "./include/*")
file(GLOB_RECURSE SOURCES "./sources/*.cpp")

# Library target
add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC ptree config++)

add_subdirectory(tests)
