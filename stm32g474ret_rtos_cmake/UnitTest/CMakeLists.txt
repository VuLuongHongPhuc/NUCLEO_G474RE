cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_COMPILER "gcc.exe")
set(CMAKE_CXX_COMPILER "g++.exe")

project(MyUnitTestProject LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# Find GoogleTest installed by MSYS2
find_package(GTest REQUIRED)

# Project file
set(INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/Include)
file(COPY
    ../Utils/include/convert_to_string.h
    ../Utils/include/stream.h
    DESTINATION ${INCLUDES})
include_directories(${INCLUDES})
set(PROJECT_SRC
    ../Utils/source/convert_to_string.c
    ../Utils/source/stream.c)


# Test file
include_directories(tests/include)
file(GLOB_RECURSE TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")



# Test executable
add_executable(unit_tests
    ${TEST_SRC}
    ${PROJECT_SRC})

target_compile_options(unit_tests PRIVATE
    -DUNIT_TEST
    -Wall
    -Wextra
)

target_link_libraries(unit_tests PRIVATE
    GTest::gtest
    GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
