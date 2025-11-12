cmake_minimum_required(VERSION 3.23.1)

add_library(lldlib
        ${CMAKE_CURRENT_SOURCE_DIR}/system_stm32g4xx.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_adc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_comp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_cordic.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_crc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_crs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_dac.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_dma.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_exti.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_gpio.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_hrtim.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_i2c.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_lptim.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_lpuart.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_opamp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_pwr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_rcc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_rng.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_rtc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_spi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_tim.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_usart.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32g4xx_ll_utils.c
#        PARENT_SCOPE
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