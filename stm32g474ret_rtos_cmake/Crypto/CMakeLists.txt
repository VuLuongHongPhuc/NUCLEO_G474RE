cmake_minimum_required(VERSION 3.23.1)

set(AES_LIB_NAME "crypto_aes_lib")

add_library(${AES_LIB_NAME} STATIC
        aes.c
)

target_compile_definitions(${AES_LIB_NAME} PRIVATE
        -DCBC
)

target_include_directories(${AES_LIB_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(${AES_LIB_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:C>:-mcpu=cortex-m4>
        $<$<COMPILE_LANGUAGE:C>:-g3>
        $<$<COMPILE_LANGUAGE:C>:-c>
        $<$<COMPILE_LANGUAGE:C>:-Os>
        $<$<COMPILE_LANGUAGE:C>:-ffunction-sections>
        $<$<COMPILE_LANGUAGE:C>:-fdata-sections>
        $<$<COMPILE_LANGUAGE:C>:-Wl,--gc-sections>
        $<$<COMPILE_LANGUAGE:C>:-Wall>
        $<$<COMPILE_LANGUAGE:C>:-fstack-usage>
        $<$<COMPILE_LANGUAGE:C>:--specs=nano.specs>
        $<$<COMPILE_LANGUAGE:C>:-mfpu=fpv4-sp-d16>
        $<$<COMPILE_LANGUAGE:C>:-mfloat-abi=hard>
        $<$<COMPILE_LANGUAGE:C>:-mthumb>
        $<$<COMPILE_LANGUAGE:C>:-mlittle-endian>
)