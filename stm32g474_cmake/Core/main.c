

/*
 * FW_G4_V1.5.1
 * */


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include <stm32g474xx.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_iwdg.h>

#include "main.h"
#include "Initialize.h"
#include "system.h"
#include "gpio.h"
#include "lpuart1.h"
#include "crc.h"

//C:\Users\admin\STM32Cube\Repository\STM32Cube_FW_G4_V1.5.1

static uint32_t tab[] = {
	0x12345678,
	0x3456789A,
	0x56789ABC,
	0x789ABCDE,
};

__STATIC_INLINE uint32_t GetElapseTime(uint32_t tick, uint32_t value)
{
	if (tick >= value)
	{
		return (uint32_t)(tick - value);
	}
	else
	{
		return (uint32_t)(tick + (UINT32_MAX - value));
	}
}

int main(void)
{
	static LL_RCC_ClocksTypeDef clock_ref = {0};

	uint32_t last_time;
	uint32_t time_elapse;

	GLOBAL_Initialize();

	/* get clocks frequencies */
	LL_RCC_GetSystemClocksFreq(&clock_ref);

	printf("System started.\r\n");

	last_time = GetSysTick();

	while (1)
	{
		time_elapse = GetSysTick() - last_time;
		//time_elapse = GetElapseTime(GetSysTick(), last_time);

		if (time_elapse > 1000)
		{
			last_time = GetSysTick();

			/* Toggle BUILT-IN LED */
			LL_GPIO_TogglePin(BUILT_IN_LED_Port, BUILT_IN_LED_Pin);

			#if 0
			time_elapse = GetSysCoreClockCount();
			printf("%d\r\n", (int)time_elapse);
			#endif

			#if 0
			//LPUART1_WriteOneData('A');
			uint8_t msg[] = "Hello from LPUART1!\r\n";
			LPUART1_Write( msg, sizeof(msg)/sizeof(msg[0]));

			uint16_t nLu = LPUART1_Read(msg, 1);
			if (nLu)
			{
				printf("%c\r\n", msg[0]);
			}
			#endif

			// Refresh watchdog
			//LL_IWDG_ReloadCounter(IWDG);


			uint32_t crc = CRC_Compute( (uint32_t*)tab, 1);
			printf("%x\r\n", (int)crc);
		}

	}// while(1)
}


void Error_Handler(void)
{
	/* User can add his own implementation to report the HAL error return state */

	__disable_irq();
	while(1);
}



/*EOF*/
