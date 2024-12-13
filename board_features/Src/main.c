

/* FW_F1_V1.8.5
 * FW_G4_V1.5.1
 * */

//C:\Users\admin\STM32Cube\Repository\STM32Cube_FW_G4_V1.5.1

/* Includes ------------------------------------------------------------------*/

#include <stm32g474xx.h>
#include <stm32g4xx_ll_iwdg.h>
#include <stm32g4xx_ll_usart.h>
#include <stm32g4xx_ll_gpio.h>

#include "main.h"
#include "Initialize.h"
#include "system.h"
#include "stream.h"
#include "cli_task.h"
#include "gpio.h"
#include "console.h"


int main(void)
{
	uint32_t last_time;
	uint32_t time_elapse;

	// System / hardware initialize
	Initialize();

	// Task initialize
	CLI_Initialize();

	last_time = GetSysTick();

	LL_GPIO_SetOutputPin(BUILT_IN_LED_Port, BUILT_IN_LED_Pin);

	while (1)
	{
		time_elapse = GetSysTick() - last_time;

		if (time_elapse > 1000)
		{
			last_time = GetSysTick();

			/* Toggle BUILT-IN LED */
			//LL_GPIO_TogglePin(BUILT_IN_LED_Port, BUILT_IN_LED_Pin);

			/* Reset BUILT-IN LED */
			//LL_GPIO_ResetOutputPin(BUILT_IN_LED_Port, BUILT_IN_LED_Pin);

			// Software trigger EXTI user button
			//EXTI->SWIER1 |= 1UL << 13;

			/* Update the counter value */
			LL_IWDG_ReloadCounter(IWDG);

			//DEBUG_WriteHex(GetSysCoreClockCount(), 10);
		}

#if 0
			/* USER button press */
			//LL_GPIO_ReadInputPort(GPIOC)
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

		CLI_task();

	}// end while(1)
}


void Error_Handler(void)
{
	/* User can add his own implementation to report the HAL error return state */

	__disable_irq();
	while(1);
}


/*EOF*/
