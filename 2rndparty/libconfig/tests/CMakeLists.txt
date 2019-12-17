# Имя библиотеки имплементации
set(PROJECT_IMPL ${PROJECT_NAME})
project(test${PROJECT_NAME})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Libs
set(LIBS
        gtest
        gmock
        pthread
        ${PROJECT_IMPL}
        )

FILE(GLOB_RECURSE TESTS_SOURCES ./*.cpp)
FILE(GLOB_RECURSE TESTS_SUPPLY ./*.hpp)
FILE(GLOB_RECURSE HEADERS ./*.h)

add_executable(${PROJECT_NAME} ${TESTS_SOURCES} ${TESTS_SUPPLY} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${LIBS})
add_test(NAME ${PROJECT_IMPL} COMMAND ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE GTEST_LINKED_AS_SHARED_LIBRARY)
