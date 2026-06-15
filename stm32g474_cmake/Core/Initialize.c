/*
 * Initialize.c
 *
 *  Created on: May 12, 2024
 *      Author: admin
 */


#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_bus.h>
#include "Initialize.h"
#include "system.h"
#include "gpio.h"
#include "lpuart1.h"
#include "watchdog.h"
#include "usart1.h"


/********************************* Inline prototypes ******************************/



/********************************* Implementations ********************************/

void GLOBAL_Initialize(void)
{
	LL_RCC_ClocksTypeDef clock_ref = {0};

	SYSTEM_Initialize();

	/* Get clock */
	LL_RCC_GetSystemClocksFreq(&clock_ref);

	GPIO_Initialize();
	LPUART1_Initialize();
	USART1_Initialize(&clock_ref.PCLK2_Frequency);
	//WATCHDOG_Initialize();
	
}


/*EOF*/
