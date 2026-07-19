# Project build
Board NUCLEO-G474RE
## Build tool
- cmake v3.23
- arm compiler 14.2
## Build command
- create folder "bin": $ mkdir bin
- go into folder: $ cd bin
- generate makefiles: $ cmake ..
- build: $ cmake --build .

## Pinout
| NAME         | Pin name | Pin number |
|--------------|----------|------------|
| LPUART1_RX   | PA3      | 17         |
| LPUART1_TX   | PA2      | 14         |
| BUILT_IN_LED | PA5      | 19         |
| BTN_USER     | PC13     |  2         |
| USART1_TX    | PC4      | 22         |
| USART1_RX    | PC5      | 23         |
| FDCAN1_RX    | PA11     | 45         |
| FDCAN1_TX    | PA12     | 46         |

## DMA
| NAME         | Channel x | Assign to  |
|--------------|-----------|------------|
| DMA1         | channel 1 | USART1_RX  |

## LIBRARIES
- freeRTOS Kernel V11.3.0
- tiny-AES-c https://github.com/kokke/tiny-AES-c?utm_source=chatgpt.com
- stm32 LL STM32Cube_FW_G4_V1.5.1

## WSL ubuntu dev
### Install ARM compiler
```text
sudo apt update
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi gdb-multiarch
```
- check version
```text
arm-none-eabi-gcc --version
```
- This provides:
<pre>
arm-none-eabi-gcc — C compiler
arm-none-eabi-g++ — C++ compiler
arm-none-eabi-as — assembler
arm-none-eabi-ld — linker
arm-none-eabi-objcopy — binary/hex generation
arm-none-eabi-gdb — debugger
</pre>
### Install build tools
```text
sudo apt install build-essential cmake ninja-build make git
```