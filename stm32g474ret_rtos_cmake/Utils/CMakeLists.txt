cmake_minimum_required(VERSION 3.23.1)

file(GLOB_RECURSE UTILS_SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")

set(UTILS_SRC
        ${UTILS_SOURCE_FILE}
        PARENT_SCOPE
)

#set(UTILS_SRC
#        ${CMAKE_CURRENT_SOURCE_DIR}/stream.c
#        ${CMAKE_CURRENT_SOURCE_DIR}/convert_to_string.c
#        PARENT_SCOPE
#)
