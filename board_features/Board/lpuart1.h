/*
 * lpuart1.h
 *
 *  Created on: Dec 7, 2024
 *      Author: admin
 */

#ifndef SRC_LPUART1_H_
#define SRC_LPUART1_H_


/*
 * RM0440
 * p.1590 --- $37   USART/UART
 * p.1640 --- $37.7 USART interrupts
 *
 * p.1677 --- $38     LPUART
 * p.1684 --- $38.4.5 LPUART transmit
 *
 * p.1705 --- $ LPUART registers
 *
 * DS12288
 * p.73   --- $ ALTERNATE function
 * */

#include <stdint.h>
#include "stream.h"

void LPUART1_Initialize(void);
void LPUART1_WriteOneData(uint8_t value);
void LPUART1_Write(const uint8_t* buff, uint8_t length);
void LPUART1_Read(uint8_t* buffer, uint8_t length, uint8_t* read);
Stream_t* LPUART1_GetRxStream(void);



#endif /* SRC_LPUART1_H_ */
