/*
 * Initialize.c
 *
 *  Created on: May 12, 2024
 *      Author: admin
 */


#include "stm32g4xx_ll_bus.h"
#include "Initialize.h"


void InitializeGPIO()
{

	// LED_BUILTIN PA5 D13 pin.19
	// BLUE BUTTON PC13 pin.2

	// page 351 $ 9. GPIO

	// MODE(i) = Input, Output, Alternate, Analog
	// PUPD(i) = OD, PP, PU, PD
	// ODR = output write
	// IDR = input read
	// AFRL & AFRH = alternate pin configuration
	// MODER = alternate mode configuration

	// Digital configuration order:
	// 1. MODER
	// 2. AFRL & AFRH
	// 3. OTYPER, PUPDR, OSPEEDER

	// Other: ADC, DAC, OPAMP, COMP
	// 1. MODER
	// 2. there respective register

	// Special: RTC, PWR, RCC

	// Write to ouput with BSRR $ 9.3.5 I/O data bitwise handling





	// enable clock
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);


	/* LED_BUILTIN PA5 setting */

	// IO mode = 01 -> output (default 11)
	//GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
	//GPIOA->MODER |= GPIO_MODER_MODE5_0;
	GPIOA->MODER &= (~GPIO_MODER_MODE5_Msk | GPIO_MODER_MODE5_0);

	// IO type
	// GPIOA->OTYPER = 0 default -> push-pull

	// IO speed
	// GPIOA->OSPEEDR = 00 default -> Low speed

	// IO pull-up / pull-down
	// GPIOA->PUPDR = 00 default -> No pull-up, pull down

	// Lock the IO configuration
	// GPIOA->LCKR = 0 default -> not locked

	/* BLUE BUTTON PC13 setting */
	GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk;
}

/*EOF*/
