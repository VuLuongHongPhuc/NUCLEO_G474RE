/*
 * Initialize.c
 *
 *  Created on: May 12, 2024
 *      Author: admin
 */


#include "Initialize.h"
#include "system.h"

#include "gpio.h"
#include "lpuart1.h"
#include "watchdog.h"
#include "../LL/stm32g4xx_ll_bus.h"
#include "../LL/stm32g4xx_ll_rcc.h"


static LL_RCC_ClocksTypeDef clock_ref = {0};


void Initialize(void)
{
	SYSTEM_Initialize();

	/* get clocks frequencies */
	LL_RCC_GetSystemClocksFreq(&clock_ref);

	GPIO_Initialize();
	LPUART1_Initialize();
	WATCHDOG_Initialize();
}


/*EOF*/
