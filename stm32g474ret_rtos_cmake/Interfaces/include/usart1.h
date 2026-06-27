/**
 * @file usart1.h
 * @brief USART1 interface
 * 
 * @author Phuc VU
 * @date Jun 13, 2026
 */

#ifndef TEMPLATE_H_
#define TEMPLATE_H_

/********************************* Includes ***************************************/
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/********************************* Macros *****************************************/

/********************************* Data structure *********************************/

/********************************* Global variables *******************************/

/********************************* Inline prototypes ******************************/

/********************************* Prototypes *************************************/

/**
 * @brief USART1 initialization
 * @param pclk2_frequency - PCLK2 frequency
 * @retval None
 * */
extern void USART1_Initialize(const uint32_t * const pclk2_frequency);

extern bool USART1_Write(const uint8_t * const pBuf, uint16_t length);

/********************************* Inline implementations *************************/


#ifdef __cplusplus
}
#endif

#endif /* TEMPLATE_H_ */