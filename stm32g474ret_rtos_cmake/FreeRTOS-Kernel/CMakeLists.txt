cmake_minimum_required(VERSION 3.15)

# portable -> GCC_ARM_CM4F - Compiler: GCC - Target: ARM Cortex-M4

set(FREERTOS_KERNEL_LIB_NAME    "freertos_kernel_lib")

file(GLOB_RECURSE FREERTOS_SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")

add_library(${FREERTOS_KERNEL_LIB_NAME} STATIC ${FREERTOS_SOURCE_FILE})

target_include_directories(${FREERTOS_KERNEL_LIB_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_compile_options(${FREERTOS_KERNEL_LIB_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-mcpu=cortex-m4>
    $<$<COMPILE_LANGUAGE:C>:-mlittle-endian>
    $<$<COMPILE_LANGUAGE:C>:-g3>
    $<$<COMPILE_LANGUAGE:C>:-c>
    $<$<COMPILE_LANGUAGE:C>:-O0>
    $<$<COMPILE_LANGUAGE:C>:-ffunction-sections>
    $<$<COMPILE_LANGUAGE:C>:-fdata-sections>
    $<$<COMPILE_LANGUAGE:C>:-Wall>
    $<$<COMPILE_LANGUAGE:C>:-Wextra>
    $<$<COMPILE_LANGUAGE:C>:-Wpedantic>
    $<$<COMPILE_LANGUAGE:C>:-Wconversion>
    $<$<COMPILE_LANGUAGE:C>:-fstack-usage>
    $<$<COMPILE_LANGUAGE:C>:--specs=nano.specs>
    $<$<COMPILE_LANGUAGE:C>:-mfpu=fpv4-sp-d16>
    $<$<COMPILE_LANGUAGE:C>:-mfloat-abi=hard>
    )

