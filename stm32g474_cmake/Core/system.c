/*
 * system.c
 *
 *  Created on: May 12, 2024
 *      Author: admin
 */



/********************************* Includes ***************************************/
#include <stdint.h>
#include "stm32g474xx.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_utils.h"
#include "system.h"




/********************************* Macros *****************************************/
#define  TICK_INT_PRIORITY           ((1UL << __NVIC_PRIO_BITS) - 1UL)    /*!< tick interrupt priority (lowest by default)  */

typedef enum
{
  HAL_TICK_FREQ_10HZ         = 100U,
  HAL_TICK_FREQ_100HZ        = 10U,
  HAL_TICK_FREQ_1KHZ         = 1U,
  HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ
} HAL_TickFreqTypeDef;


/********************************* Global variables *******************************/
uint32_t g_uwTick = 0;

/********************************* Private variables ******************************/
static uint32_t _uwTickPrio = (1UL << __NVIC_PRIO_BITS); /* Invalid PRIO */
static HAL_TickFreqTypeDef _uwTickFreq = HAL_TICK_FREQ_DEFAULT;



/********************************* Private prototypes *****************************/

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follows :
 *        System Clock source            = PLL (HSE=24 MHz)
 *        SYSCLK(Hz)                     = 100000000 (100MHz)
 *        HCLK(Hz)                       = 100000000 (100MHz)
 *        AHB Prescaler                  = 2
 *        APB1 Prescaler                 = 4
 *        APB2 Prescaler                 = 4
 *        HSE Frequency(Hz)              = 24000000 (24MHz)
 *        PLL_M                          = 3
 *        PLL_N                          = 25
 *        PLL_R                          = 2
 */
static void SystemClockInit(void);

/**
 * @brief  SysTick Configuration
 *         The SysTick clock source is set to HCLK.
 */
static void SysTickInit(void);



/********************************* Inline prototypes ******************************/
__STATIC_FORCEINLINE void EnableCoreSysClock(void);
__STATIC_FORCEINLINE uint32_t GetSysTickPriority(void);


/********************************* Implementations ********************************/

/**
 * @brief  This function is called on program startup just after reset and before
 *         branching to main program.
 * @note   Initialises the standard library functions.
 * @retval None
 */
void __libc_init_array(void)
{

}

void SYSTEM_Initialize()
{
	SystemClockInit();

	/* Set Interrupt Group Priority : NVIC_PRIORITYGROUP_4 -> 0..15 (0=highest priority) */
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	SysTickInit();
}



static void SystemClockInit(void)
{

	/*
	 * HSE    = 24 MHz -> Set in stm32g4xx_ll_rcc.h
	 * SYSCLK = 100 MHz
	 * HCLK   = 100 MHz
	 * PCLK1  = 25 MHz
	 * PCLK2  = 25 MHz
	 * ABP1 timer = 50 MHz
	 * ABP2 timer = 50 MHz
	 * */


    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral */
    LL_PWR_DisableUCPDDeadBattery();

    LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
    while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_4)
    {
    }
    LL_PWR_EnableRange1BoostMode();
    LL_RCC_HSE_Enable();
    /* Wait till HSE is ready */
    while(LL_RCC_HSE_IsReady() != 1)
    {
    }

    LL_RCC_HSI_Enable();
    /* Wait till HSI is ready */
    while(LL_RCC_HSI_IsReady() != 1)
    {
    }
    LL_RCC_HSI_SetCalibTrimming(64);

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_3, 25, LL_RCC_PLLR_DIV_2);
    LL_RCC_PLL_EnableDomain_SYS();
    LL_RCC_PLL_Enable();
    /* Wait till PLL is ready */
    while(LL_RCC_PLL_IsReady() != 1)
    {
    }

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    /* Wait till System clock is ready */
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    }

    /* Set AHB prescaler*/
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_4);

    // Set to core register the system clock frequency
    LL_SetSystemCoreClock(BOARD_SYSTEM_CORE_CLOCK);
}

static void SysTickInit(void)
{
  /* SysTick set to 1 ms */
    uint32_t tick_time = SystemCoreClock / (1000U / _uwTickFreq);

    if (!SysTick_Config(tick_time))
    {/* succeeded */
      /* Configure the SysTick IRQ priority - value set in SysTick_Config */
      _uwTickPrio = TICK_INT_PRIORITY;
    }

#if 0
  uint32_t TickPriority = TICK_INT_PRIORITY;
	// duplicate of SysTick_Config to set _uwTickPrio
	/* Configure the SysTick IRQ priority */
	if (TickPriority < (1UL << __NVIC_PRIO_BITS))
	{
		uint32_t prioritygroup = NVIC_GetPriorityGrouping();

		NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(prioritygroup, TickPriority, 0U));

		_uwTickPrio = TickPriority;
	}
#endif
}

/**
 * @brief  This IRQ handler function is called to increment a global variable "g_uwTick"
 *         used as application time base.
 */
void SysTick_Handler(void)
{
	g_uwTick += _uwTickFreq;
}


/********************************* Inline implementation **************************/
__STATIC_FORCEINLINE uint32_t GetSysTickPriority(void)
{
  return _uwTickPrio;
}


__STATIC_FORCEINLINE void EnableCoreSysClock(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}



/*EOF*/
