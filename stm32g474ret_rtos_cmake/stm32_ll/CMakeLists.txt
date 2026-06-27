cmake_minimum_required(VERSION 3.23.1)

set(STM32_ll_LIB_NAME    "lld_lib")

file(GLOB_RECURSE LLD_SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")

add_library(${STM32_ll_LIB_NAME}
        STATIC
        ${LLD_SOURCE_FILE}
)

target_compile_definitions(${STM32_ll_LIB_NAME} PRIVATE
        -DSTM32G474xx
        -DUSE_FULL_LL_DRIVER
        )

target_include_directories(${STM32_ll_LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(${STM32_ll_LIB_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:C>:-mcpu=cortex-m4>
        $<$<COMPILE_LANGUAGE:C>:-g3>
        $<$<COMPILE_LANGUAGE:C>:-c>
        $<$<COMPILE_LANGUAGE:C>:-O2>
        $<$<COMPILE_LANGUAGE:C>:-ffunction-sections>
        $<$<COMPILE_LANGUAGE:C>:-fdata-sections>
        $<$<COMPILE_LANGUAGE:C>:-Wall>
        $<$<COMPILE_LANGUAGE:C>:-fstack-usage>
        $<$<COMPILE_LANGUAGE:C>:--specs=nano.specs>
        $<$<COMPILE_LANGUAGE:C>:-mfpu=fpv4-sp-d16>
        $<$<COMPILE_LANGUAGE:C>:-mfloat-abi=hard>
        $<$<COMPILE_LANGUAGE:C>:-mthumb>
        $<$<COMPILE_LANGUAGE:C>:-mlittle-endian>
        )