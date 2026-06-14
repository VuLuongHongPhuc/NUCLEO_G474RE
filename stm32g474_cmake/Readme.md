# Project build
Board NUCLEO-G474RE
## Build tool
- cmake v3.23
- arm compiler 14.2
## Build command
- create folder "bin" : $ mkdir bin
- go into folder it: $ cd bin
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