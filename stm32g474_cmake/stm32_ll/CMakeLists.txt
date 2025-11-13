cmake_minimum_required(VERSION 3.23.1)


file(GLOB_RECURSE LLD_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

add_library(lldlib
        STATIC
        ${LLD_SOURCES}
)

target_compile_definitions(lldlib PRIVATE
        -DSTM32G474xx
        -DUSE_FULL_LL_DRIVER
#        -DDEBUG
        )

target_include_directories(lldlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(lldlib PRIVATE
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