

/* FW_F1_V1.8.5
 * FW_G4_V1.5.1
 * */


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "stm32g474xx.h"
#include "stm32g4xx_ll_rcc.h"

#include "main.h"
#include "Initialize.h"
#include "system.h"

//C:\Users\admin\STM32Cube\Repository\STM32Cube_FW_G4_V1.5.1


static LL_RCC_ClocksTypeDef clock_ref = {0};

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
	uint32_t last_time;
	uint32_t time_elapse;

	InitSystem();
	InitializeGPIO();

	/* get clocks frequencies */
	LL_RCC_GetSystemClocksFreq(&clock_ref);

	last_time = GetSysTick();

	while (1)
	{
		time_elapse = GetSysTick() - last_time;
		//time_elapse = GetElapseTime(GetSysTick(), last_time);

		if (time_elapse > 1000)
		{
			last_time = GetSysTick();

			/* Toggle BUILT-IN LED */
			GPIOA->ODR ^= (1UL << 5);

			time_elapse = GetSysCoreClockCount();

			printf("%d\r\n", (int)time_elapse);
		}

#if USER_BUTTON_PRESS
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
	}// end while(1)
}


void Error_Handler(void)
{
	/* User can add his own implementation to report the HAL error return state */

	__disable_irq();
	while(1);
}



/*EOF*/
