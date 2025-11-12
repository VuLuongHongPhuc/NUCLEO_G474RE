/*
 * Initialize.c
 *
 *  Created on: May 12, 2024
 *      Author: admin
 */


#include "stm32g4xx_ll_bus.h"
#include "Initialize.h"
#include "system.h"
#include "gpio.h"
#include "lpuart1.h"
#include "watchdog.h"
#include "crc.h"

/********************************* Inline prototypes ******************************/



/********************************* Implementations ********************************/

void GLOBAL_Initialize(void)
{
	SYSTEM_Initialize();
	GPIO_Initialize();
	LPUART1_Initialize();
	//WATCHDOG_Initialize();
	CRC_Initialize();
}


/*EOF*/
