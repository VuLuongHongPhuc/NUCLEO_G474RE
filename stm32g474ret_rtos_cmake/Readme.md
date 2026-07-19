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
