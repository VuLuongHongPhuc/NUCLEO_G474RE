cmake_minimum_required(VERSION 3.23.1)

set(INTERFACES_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/gpio.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lpuart1.c
        ${CMAKE_CURRENT_SOURCE_DIR}/watchdog.c
        ${CMAKE_CURRENT_SOURCE_DIR}/crc.c
        PARENT_SCOPE
)

#target_include_directories(INTERFACES_SRC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
