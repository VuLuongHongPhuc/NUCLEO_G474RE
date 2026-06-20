cmake_minimum_required(VERSION 3.23.1)

set(UTILS_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/stream.c
        ${CMAKE_CURRENT_SOURCE_DIR}/convert_to_string.c
        PARENT_SCOPE
)
