/*
 * gpio.h
 *
 *  Created on: Dec 7, 2024
 *      Author: admin
 */

#ifndef BOARD_GPIO_H_
#define BOARD_GPIO_H_


/*
 * RM0440
 * p.351 GPIO
 * */

#include "stm32g474xx.h"

/**
 * BUILT_IN LED PA5
 */
#define BUILT_IN_LED_Pin		LL_GPIO_PIN_5
#define BUILT_IN_LED_Port		GPIOA
#define BUILT_IN_LED_Mode		LL_GPIO_MODE_OUTPUT

/**
 * Toggle output
 * GPIOA->ODR ^= (1UL << LL_GPIO_PIN_5);
 * LL_GPIO_TogglePin
 *
 * Set
 * GPIOA->BSRR = (1UL << GPIO_BSRR_BS5);
 * LL_GPIO_SetOutputPin
 *
 * clear
 * GPIOA->BSRR = (1UL << GPIO_BSRR_BR5);
 * LL_GPIO_ResetOutputPin
 */

/*
 * BUILT_IN BUTTON USER PC13
 * GPIOC, GPIO_PIN_13
 * */

#define BUILT_IN_USER_Pin		LL_GPIO_PIN_13
#define BUILT_IN_USER_Port		GPIOC
#define BUILT_IN_USER_Mode		LL_GPIO_MODE_INPUT


void GPIO_Initialize(void);

#endif /* BOARD_GPIO_H_ */
