

/*
 * STM32Cube_FW_G4_V1.5.1
 * C:\Users\xxx\STM32Cube\Repository\STM32Cube_FW_G4_V1.5.1\Drivers\STM32G4xx_HAL_Driver\Src
 * C:\Users\xxx\STM32Cube\Repository\STM32Cube_FW_G4_V1.5.1\Drivers\STM32G4xx_HAL_Driver\Inc
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

	/* DEBUG - get clocks frequencies */
	LL_RCC_GetSystemClocksFreq(&clock_ref);
	printf("SYSTEM frequencies:\r\n");
	printf(" - SYSCLK: %d\r\n", (int)clock_ref.SYSCLK_Frequency);
	printf(" - HCLK  : %d\r\n", (int)clock_ref.HCLK_Frequency);
	printf(" - PCLK1 : %d\r\n", (int)clock_ref.PCLK1_Frequency);
	printf(" - PCLK2 : %d\r\n", (int)clock_ref.PCLK2_Frequency);

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
			LPUART1_TestTransmit();
			LPUART1_TestReceive();
			#endif
			
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
