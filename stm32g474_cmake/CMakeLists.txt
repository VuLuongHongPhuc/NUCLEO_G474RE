# https://embedded-dbl.com/visual-studio-code-for-arm-with-cmake-3-toolchain-cmake/
cmake_minimum_required(VERSION 3.23.1)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/Buildtools.cmake)

# Create a project named
project(STM32_G474RET)
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(LINKER_FILE ${CMAKE_SOURCE_DIR}/STM32G474RETX.ld)

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

# Add subdirectories - order matters
add_subdirectory(LL)
add_subdirectory(Utils)
add_subdirectory(Interfaces)

# Add the source file to the project
# and set the variable PROJECT_SOURCES to this source file
set(PROJECT_SOURCES
        ${UTILS_SRC}
        ${INTERFACES_SRC}
        Core/main.c
        Core/stm32g4xx_startup.c
        Core/Initialize.c
        Core/syscalls.c
        Core/system.c
    )

# Add an executable to the project
add_executable(${EXECUTABLE} ${PROJECT_SOURCES})
target_link_libraries(${EXECUTABLE} PRIVATE lldlib)

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${EXECUTABLE} PRIVATE
        -DSTM32G474xx
        -DUSE_FULL_LL_DRIVER
        -DDEBUG
        )

# List of include directories
target_include_directories(${EXECUTABLE} PRIVATE
        ${CMAKE_SOURCE_DIR}/Core
        ${CMAKE_SOURCE_DIR}/Interfaces
        ${CMAKE_SOURCE_DIR}/Utils
        )

# Set ASM compiler options
target_compile_options(${EXECUTABLE} PRIVATE
        $<$<COMPILE_LANGUAGE:ASM>:-mcpu=cortex-m4>
        $<$<COMPILE_LANGUAGE:ASM>:-mlittle-endian>
        $<$<COMPILE_LANGUAGE:ASM>:-g3>
#        $<$<COMPILE_LANGUAGE:ASM>:-DDEBUG>
        $<$<COMPILE_LANGUAGE:ASM>:-x>
        $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>
        $<$<COMPILE_LANGUAGE:ASM>:--specs=nano.specs>
        $<$<COMPILE_LANGUAGE:ASM>:-mfpu=fpv4-sp-d16>
        $<$<COMPILE_LANGUAGE:ASM>:-mfloat-abi=hard>
        $<$<COMPILE_LANGUAGE:ASM>:-mthumb>
        )

# Compiler options for C language only
target_compile_options(${EXECUTABLE} PRIVATE
        $<$<COMPILE_LANGUAGE:C>:-mcpu=cortex-m4>
        $<$<COMPILE_LANGUAGE:C>:-mlittle-endian>
        $<$<COMPILE_LANGUAGE:C>:-g3>
#        $<$<COMPILE_LANGUAGE:C>:-DDEBUG>
        $<$<COMPILE_LANGUAGE:C>:-c>
        $<$<COMPILE_LANGUAGE:C>:-O0>
        $<$<COMPILE_LANGUAGE:C>:-ffunction-sections>
        $<$<COMPILE_LANGUAGE:C>:-fdata-sections>
        $<$<COMPILE_LANGUAGE:C>:-Wall>
        $<$<COMPILE_LANGUAGE:C>:-fstack-usage>
#        $<$<COMPILE_LANGUAGE:C>:-fcyclomatic-complexity>
        $<$<COMPILE_LANGUAGE:C>:--specs=nano.specs>
        $<$<COMPILE_LANGUAGE:C>:-mfpu=fpv4-sp-d16>
        $<$<COMPILE_LANGUAGE:C>:-mfloat-abi=hard>
        $<$<COMPILE_LANGUAGE:C>:-mthumb>
)

# Compiler options for C++ language only
target_compile_options(${EXECUTABLE} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-mcpu=cortex-m4>
        $<$<COMPILE_LANGUAGE:CXX>:-mlittle-endian>
        $<$<COMPILE_LANGUAGE:CXX>:-g3>
#        $<$<COMPILE_LANGUAGE:CXX>:-DDEBUG>
        $<$<COMPILE_LANGUAGE:CXX>:-c>
        $<$<COMPILE_LANGUAGE:CXX>:-O0>
        $<$<COMPILE_LANGUAGE:CXX>:-ffunction-sections>
        $<$<COMPILE_LANGUAGE:CXX>:-fdata-sections>
        $<$<COMPILE_LANGUAGE:CXX>:-Wall>
        $<$<COMPILE_LANGUAGE:CXX>:-fstack-usage>
        $<$<COMPILE_LANGUAGE:CXX>:--specs=nano.specs>
        $<$<COMPILE_LANGUAGE:CXX>:-mfpu=fpv4-sp-d16>
        $<$<COMPILE_LANGUAGE:CXX>:-mfloat-abi=hard>
        $<$<COMPILE_LANGUAGE:CXX>:-mthumb>
)

# Linker options
# Explanation of some options:
# -u 
# _printf_float -> enable float support in printf (functionality increases code size)
#                     use of this option requires --specs=rdimon.specs
#                     call syscalls.c to provide semihosting support functions
# --specs=rdimon.specs -> enable semihosting support
# --specs=nano.specs -> use newlib-nano (smaller C library implementation)
# --specs=nosys.specs -> use minimal system calls (no OS)
# -Wl,--gc-sections -> remove unused sections
# -nostdlib -> do not use standard libraries
# -static -> do not use shared libraries
# -Wl,--start-group ... -Wl,--end-group -> resolve circular dependencies between libraries
# -Xlinker -print-memory-usage -> print memory usage after linking
target_link_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m4
        -T${LINKER_FILE}
        -mlittle-endian
        --specs=nosys.specs
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
        -nostdlib
        -static
        --specs=rdimon.specs
        --specs=nano.specs
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -u
        _printf_float
        -mthumb
        -Wl,--start-group
        -lc
        -lm
        -Wl,--end-group
        -Xlinker -print-memory-usage -Xlinker
        )

# Optional: Print executable size as part of the post build process
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE})

# Optional: Create hex, bin and S-Record files after the build
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O srec --srec-len=64 ${EXECUTABLE} ${PROJECT_NAME}.s19
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)