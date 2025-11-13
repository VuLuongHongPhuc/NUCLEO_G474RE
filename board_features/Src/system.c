/*
 * system.c
 *
 *  Created on: May 12, 2024
 *      Author: admin
 */


/* Includes -----------------------------------------------------------------*/
#include <stdint.h>
#include "system.h"
#include "../LL/stm32g474xx.h"
#include "../LL/stm32g4xx_ll_bus.h"
#include "../LL/stm32g4xx_ll_pwr.h"
#include "../LL/stm32g4xx_ll_rcc.h"
#include "../LL/stm32g4xx_ll_system.h"
#include "../LL/stm32g4xx_ll_utils.h"




/* Private defines -----------------------------------------------------------*/
#define  TICK_INT_PRIORITY           (15UL)    /*!< tick interrupt priority (lowest by default)  */




typedef enum
{
  HAL_TICK_FREQ_10HZ         = 100U,
  HAL_TICK_FREQ_100HZ        = 10U,
  HAL_TICK_FREQ_1KHZ         = 1U,
  HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ
} HAL_TickFreqTypeDef;



/* Private variables -----------------------------------------------------------*/
static uint32_t uwTickPrio = (1UL << __NVIC_PRIO_BITS); /* Invalid PRIO */
static uint32_t uwTick = 0;
HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;



/* Private prototypes -----------------------------------------------------------*/
static void InitSystemClock();
static void InitSysTick();
static inline void EnableCoreSysClock();
static void InitMemory(void);




/* Functions -----------------------------------------------------------*/

inline uint32_t GetSysTick() { return uwTick; }
inline uint32_t GetSysTickPriority() { return uwTickPrio; }




void __libc_init_array(void)
{

}




void SYSTEM_Initialize()
{
	InitSystemClock();

	/* Set Interrupt Group Priority */
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	InitMemory();

	InitSysTick();
}



static void InitSystemClock(void)
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


    /* p.258 UCPD1_DBDIS : Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral */
    LL_PWR_DisableUCPDDeadBattery();

    /* 4WS (Wait State) good for all config */
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);

    /* Check that the new number of wait states is taken into account */
    while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_4);

    /* Recommended R1MODE = 0 if SYSCLK < 170MHz */
    if (PWR->CR5 & PWR_CR5_R1MODE)
    {
    	/* Switch from normal to boost mode step */

    	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
    	__asm__("NOP");

    	LL_PWR_EnableRange1BoostMode();
    }


    /*
     * Bit 16 HSEON: HSE clock enable
     * Set and cleared by software. Cleared by hardware to stop the HSE oscillator when entering Stop, Standby or Shutdown
     * mode. This bit cannot be reset if the HSE oscillator is used directly or indirectly as the system clock...?
     * 0: HSE oscillator OFF
     * 1: HSE oscillator ON
     * */
    LL_RCC_HSE_Enable();
    /* Wait till HSE is ready */
    while(LL_RCC_HSE_IsReady() != 1);


    LL_RCC_HSI_Enable();
    /* Wait till HSI is ready */
    while(LL_RCC_HSI_IsReady() != 1);
    LL_RCC_HSI_SetCalibTrimming(64);

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_3, 25, LL_RCC_PLLR_DIV_2);
    LL_RCC_PLL_EnableDomain_SYS();
    LL_RCC_PLL_Enable();
    /* Wait till PLL is ready */
    while(LL_RCC_PLL_IsReady() != 1);


    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    /* Wait till System clock is ready */
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);


    /* Set AHB prescaler*/
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_4);

    // Set to core register the system clock frequency
    LL_SetSystemCoreClock(BOARD_SYSTEM_CORE_CLOCK);
}



/**
* @brief Configure SysTick to 1 millisecond
*/
static void InitSysTick()
{
	uint32_t TickPriority = TICK_INT_PRIORITY;
	uint32_t tick_time = SystemCoreClock / (1000U / uwTickFreq);


	if (SysTick_Config(tick_time) > 0U)
		return;


	// duplicate of SysTick_Config to set uwTickPrio
	/* Configure the SysTick IRQ priority */
	if (TickPriority < (1UL << __NVIC_PRIO_BITS))
	{
		uint32_t prioritygroup = NVIC_GetPriorityGrouping();

		NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(prioritygroup, TickPriority, 0U));

		uwTickPrio = TickPriority;
	}
}

/**
 * @brief Initialize I-Cache, D-Cache, Prefetch
 */
static void InitMemory(void)
{
	/* FLASH->ACR : Reset value: 0x0004 0601 = 0100 0000 0110 0000 0001 */

	/* FLASH_ARC.ICEN : default enable */
	//LL_FLASH_EnableInstCache();

	/* FLASH_ARC.DCEN : default enable */
	//LL_FLASH_EnableDataCache();

	/* FLASH_ARC.PRFTEN : default disable */
	//LL_FLASH_DisablePrefetch();
}

static inline void EnableCoreSysClock()
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

inline uint32_t GetSysCoreClockCount()
{
	return DWT->CYCCNT;
}

void SysTick_Handler(void)
{
	uwTick += uwTickFreq;
}

/*EOF*/
