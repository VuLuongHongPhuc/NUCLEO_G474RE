cmake_minimum_required(VERSION 3.23.1)

# Set the project name
project(TestCode)

# Set the languages used in the project
enable_language(C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler options
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")



# Add the source file to the project
set(PROJECT_SOURCES
        main.cpp
    )

# List of includ directories
#target_include_directories(${PROJECT_NAME} PRIVATE       #src        #inc        )

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
