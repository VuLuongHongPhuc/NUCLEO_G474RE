/**
 * @file lpuart1.h
 * @brief LPUART1 Interface
 * @details Connected to debugger ST-LINK Virtual COM Port.
 * 
 * @author Phuc VU
 * @date Dec 7, 2024
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

/********************************* Includes ***************************************/

#include <stdint.h>
#include <stdbool.h>
#include <FreeRTOS.h>
#include <stream_buffer.h>

#ifdef __cplusplus
extern "C" {
#endif

/********************************* Constants definition ***************************/

/********************************* Macros definition ******************************/

/********************************* Types definition *******************************/

/********************************* Global variable ********************************/

/********************************* API functions prototype ************************/

void LPUART1_Initialize(uint32_t pclk1_frequency);

void LPUART1_WriteOneData(uint8_t value);

bool LPUART1_Write( uint8_t const * const pBuf, uint16_t length);

uint16_t LPUART1_Read(uint8_t * pBuf, uint8_t length);


void LPUART1_TestTransmit(void);
void LPUART1_TestReceive(void);

/********************************* Inline implementations *************************/


#ifdef __cplusplus
}
#endif

#endif /* SRC_LPUART1_H_ */
