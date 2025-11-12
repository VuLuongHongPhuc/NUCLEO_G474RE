/*
 * watchdog.c
 *
 *  Created on: Dec 12, 2024
 *      Author: admin
 */

#include <stm32g4xx_ll_iwdg.h>
#include "watchdog.h"




void WATCHDOG_Initialize(void)
{
	// clock default --> LSI = 32kHz

	// LSI * CounterValue(0xFFF) / LL_IWDG_PRESCALER_16 = 2.047 seconds

	// Set prescaler
	LL_IWDG_EnableWriteAccess(IWDG);
	LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_16);

	// Set reload counter value : default value 0xFFF
	//LL_IWDG_EnableWriteAccess(IWDG);
	//LL_IWDG_SetReloadCounter(IWDG, 0x0FFF);// min=0, max=0x0FFF

	// freeze/stop counter on debug mode
	// DBGMCU or configure on debug menu

	// Start watchdog counter
	LL_IWDG_Enable(IWDG);

	// Update --> feed watchdog
	//LL_IWDG_ReloadCounter(IWDG);
}



/*EOF*/
