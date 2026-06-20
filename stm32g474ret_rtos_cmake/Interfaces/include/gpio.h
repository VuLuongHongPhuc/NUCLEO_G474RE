/**
 * @file gpio.h
 * @brief GPIO Interface
 * 
 * @author Phuc VU
 * @date Dec 7, 2024
 */


/*
 * LED_BUILTIN PA5 D13 pin.19
 * BLUE BUTTON PC13 pin.2
 * 
 * page 351 $ 9. GPIO
 * 
 * MODE(i) = Input, Output, Alternate, Analog
 * PUPD(i) = OD, PP, PU, PD
 * ODR = output write
 * IDR = input read
 * AFRL & AFRH = alternate pin configuration
 * MODER = alternate mode configuration
 * 
 * Digital configuration order:
 * 1. MODER
 * 2. AFRL & AFRH
 * 3. OTYPER, PUPDR, OSPEEDER
 * 
 * Other: ADC, DAC, OPAMP, COMP
 * 1. MODER
 * 2. there respective register
 * 
 * Special: RTC, PWR, RCC
 * 
 * Write to ouput with BSRR $ 9.3.5 I/O data bitwise handling
 * 
 * NOTE : ALTERNATE FUNCTION find in doc DS12288 p.73
 */


#ifndef BOARD_GPIO_H_
#define BOARD_GPIO_H_


/*
 * RM0440
 * p.351 GPIO
 * */

/********************************* Includes ***************************************/
#include <stm32g4xx_ll_gpio.h>

#ifdef __cplusplus
extern "C" {
#endif


/********************************* Macros *****************************************/

/* GPIO operations
 * 
 * Toggle output
 * GPIOA->ODR ^= (1UL << LL_GPIO_PIN_5);
 * LL_GPIO_TogglePin(BUILT_IN_LED_Port, BUILT_IN_LED_Pin);
 *
 * Set
 * GPIOA->BSRR = (1UL << GPIO_BSRR_BS5);
 * LL_GPIO_SetOutputPin
 *
 * clear
 * GPIOA->BSRR = (1UL << GPIO_BSRR_BR5);
 * LL_GPIO_ResetOutputPin
 */

/* BUILT_IN LED PA5 */
#define BUILT_IN_LED_Pin		LL_GPIO_PIN_5
#define BUILT_IN_LED_Port		GPIOA
#define BUILT_IN_LED_Mode		LL_GPIO_MODE_OUTPUT

/* BUILT_IN BUTTON USER PC13 - GPIOC, GPIO_PIN_13 */
#define BUILT_IN_USER_Pin		LL_GPIO_PIN_13
#define BUILT_IN_USER_Port		GPIOC
#define BUILT_IN_USER_Mode		LL_GPIO_MODE_INPUT


/********************************* Global prototypes ******************************/
void GPIO_Initialize(void);


#ifdef __cplusplus
}
#endif


#endif /* BOARD_GPIO_H_ */
