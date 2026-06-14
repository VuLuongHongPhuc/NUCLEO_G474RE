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
 * @retval None
 * */
void USART1_Initialize(void);

void USART1_Write(uint8_t value);

/********************************* Inline implementations *************************/


#ifdef __cplusplus
}
#endif

#endif /* TEMPLATE_H_ */