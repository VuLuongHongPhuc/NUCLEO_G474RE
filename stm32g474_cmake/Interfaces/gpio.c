/**
 * @file gpio.c
 * @brief GPIO Interface
 * 
 * @author Phuc VU
 * @date Dec 7, 2024
 */


/********************************* Includes ***************************************/
#include <stdio.h>

#include <stm32g474xx.h>
#include <stm32g4xx_ll_bus.h>

#include "gpio.h"


/********************************* Macro ******************************************/

// EXTI->RTSR1, EXTI->FTSR1, EXTI->EMR1, EXTI->IMR1
#define USER_BUTTON_SHIFT_REG 13



/********************************* Private variables ******************************/


/********************************* Private prototypes *****************************/
static void InitializeGPIOA(void);
static void InitializeGPIOC(void);
static void InitializeEXTI_GPIOC(void);



//void EXTI0_IRQHandler(void);
//void EXTI1_IRQHandler(void);
//void EXTI2_IRQHandler(void);
//void EXTI3_IRQHandler(void);
//void EXTI4_IRQHandler(void);
//void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);


/********************************* Implementations *******************************/

// Digital configuration order:
// 1. MODER
// 2. AFRL & AFRH
// 3. OTYPER, PUPDR, OSPEEDER


void GPIO_Initialize(void)
{

	InitializeGPIOA();
	InitializeGPIOC();
	InitializeEXTI_GPIOC();

	/* Set priority of Interrupt */
	NVIC_SetPriority(EXTI15_10_IRQn, 8);

	/* Enable IRQ in NVIC */
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

static void InitializeGPIOA(void)
{
	// Enable clock
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

	/** LED_BUILTIN PA5 setting */
	LL_GPIO_SetPinMode(BUILT_IN_LED_Port, BUILT_IN_LED_Pin, BUILT_IN_LED_Mode);

	// IO mode = 01 -> output (default 11)
	//GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
	//GPIOA->MODER |= GPIO_MODER_MODE5_0;
	//GPIOA->MODER &= (~GPIO_MODER_MODE5_Msk | GPIO_MODER_MODE5_0);

	// IO type
	// GPIOA->OTYPER = 0 default -> push-pull

	// IO speed
	// GPIOA->OSPEEDR = 00 default -> Low speed

	// IO pull-up / pull-down
	// GPIOA->PUPDR = 00 default -> No pull-up, pull down

	// IO alternate
	//if LL_GPIO_SetPinMode & LL_GPIO_MODE_ALTERNATE ---> LL_GPIO_GetAFPin_0_7 or LL_GPIO_SetAFPin_8_15

	// Lock the IO configuration until reset
	// GPIOA->LCKR = 0 default -> not locked
}

static void InitializeGPIOC(void)
{
	// GPIOx_MODER   : Reset value: 0xFFFF FFFF
	// GPIOx_OTYPER  : Reset value: 0x0000 0000
	// GPIOx_OSPEEDR : Reset value: 0x0000 0000
	// GPIOx_PUPDR   : Reset value: 0x0000 0000
	// GPIOx_IDR     : Reset value: 0x0000 XXXX
	// GPIOx_ODR     : Reset value: 0x0000 0000
	// GPIOx_BSRR    : Reset value: 0x0000 0000
	// GPIOx_LCKR    : Reset value: 0x0000 0000
	// GPIOx_AFRL    : Reset value: 0x0000 0000
	// GPIOx_AFRH    : Reset value: 0x0000 0000
	// GPIOx_BRR     : Reset value: 0x0000 0000

	// Enable clock
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

	/** USER BUTTON (blue) PC13 setting : default GND */
	LL_GPIO_SetPinMode(BUILT_IN_USER_Port, BUILT_IN_USER_Pin, BUILT_IN_USER_Mode); //MODER
	//LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_13, LL_GPIO_OUTPUT_PUSHPULL);	//OTYPER
	//LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_LOW);	//OSPEEDR
	//LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_13, LL_GPIO_PULL_NO);	//PUPDR
	//LL_GPIO_GetAFPin_0_7 / LL_GPIO_SetAFPin_8_15 --> AFR
	//LL_GPIO_LockPin(GPIOC, LL_GPIO_PIN_13);	//LCKR


}

static void InitializeEXTI_GPIOC(void)
{
	// p.450 EXTI registers

	//--- Hardware interrupt selection
	// EXTI_IMR : mask
	// EXTI_RTSR and EXTI_FTSR : trigger select
	// Enable IRQ mask

	//--- Hardware event selection
	// EXTI_EMR1 --> event masked   : 0x0000;
	// EXTI_RTSR and EXTI_FTSR : trigger select

	//--- Software interrupt/event selection
	// EXTI_IMR, EXTI_EMR : mask
	// EXTI_SWIER : bit software interrupt

	// Rising/falling edge configuration
	//EXTI->RTSR1  --> rising edge  : 0x2000 = 0010 0000 0000 0000
	//EXTI->FTSR1  --> falling edge : 0x2000 = 0010 0000 0000 0000


	// p.377 SYSCFG external interrupt configuration register : SYSCFG_EXTICR4 Reset value: 0x0000 0000
	// Set SYSCFG_EXTICR4 for USER BUTTON PC13
	SYSCFG->EXTICR[3] |= 0x0020;  /* 0b0000 0000 0010 0000 */

	/* Rising edge trigger selection register p.451 */
	EXTI->RTSR1 |= 1UL << USER_BUTTON_SHIFT_REG;   /* 0x2000 */

	/* Falling edge Trigger Selection Register p.451 */
	EXTI->FTSR1 |= 1UL << USER_BUTTON_SHIFT_REG;

	/* Interrupt Mask Register p.450 */
	EXTI->IMR1  |= 1UL << USER_BUTTON_SHIFT_REG;
}

__WEAK void UserButtonCallback(void)
{
	if (GPIOC->IDR & (1UL << USER_BUTTON_SHIFT_REG))
	{
		printf("ON\r\n");
	}
	else
	{
		printf("OFF\r\n");
	}

#if 0
		/* USER button press */
		if (GPIOC->IDR & GPIO_IDR_ID13_Msk)
		{
			// set to 1
			GPIOA->BSRR = GPIO_BSRR_BS5;
		}
		else
		{
			// reset to 0
			GPIOA->BSRR = GPIO_BSRR_BR5;
		}
#endif
}

void EXTI15_10_IRQHandler(void)
{
	
	/* p.356 $9.3.8 External interrupt/wakeup lines - related to IMR1 mask selected */ 

	/* Pending register p.454 */
	if (EXTI->PR1 & (1UL << USER_BUTTON_SHIFT_REG))
	{
		// reset flag
		EXTI->PR1 = 1UL << USER_BUTTON_SHIFT_REG;

		// Callback user button
		UserButtonCallback();
	}
}

/*EOF*/
