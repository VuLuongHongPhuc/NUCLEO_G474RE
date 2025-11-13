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

#ifdef __cplusplus
extern "C" {
#endif

/********************************* Macros *****************************************/

/********************************* Global variables *******************************/

/********************************* Inline prototypes ******************************/

/********************************* Prototypes *************************************/

/**
 * @brief LPUART initialization
 * @retval None
 * */
void LPUART1_Initialize(void);

/**
 * @brief Write one data to LPUART1
 * @param value Data to write
 * @retval None
 */
void LPUART1_WriteOneData(uint8_t value);

/**
 * @brief Write multiple data to LPUART1
 * @param pBuf Pointer to transmit buffer
 * @param length Number of data to write
 * @retval true if success, false otherwise
 */
bool LPUART1_Write( uint8_t const * const pBuf, uint16_t length);

/**
 * @brief Read data from LPUART1
 * @param pBuf Pointer to receive buffer
 * @param length Number of data to read
 * @retval Number of data actually read
 */
uint16_t LPUART1_Read(uint8_t * pBuf, uint8_t length);


void LPUART1_TestTransmit(void);
void LPUART1_TestReceive(void);

/********************************* Inline implementations *************************/


#ifdef __cplusplus
}
#endif

#endif /* SRC_LPUART1_H_ */
