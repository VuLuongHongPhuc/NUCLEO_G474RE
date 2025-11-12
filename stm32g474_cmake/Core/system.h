/*
 * system.h
 *
 *  Created on: May 12, 2024
 *      Author: admin
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

/********************************* Includes ***************************************/


#ifdef __cplusplus
extern "C" {
#endif

/********************************* Macros *****************************************/
#define BOARD_SYSTEM_CORE_CLOCK          (100000000)     /* 100 Mhz */


/** @brief Copy CORTEX_Preemption_Priority_Group CORTEX Preemption Priority Group
  */
#define NVIC_PRIORITYGROUP_0         0x00000007U /*!< 0 bit  for pre-emption priority,
                                                      4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         0x00000006U /*!< 1 bit  for pre-emption priority,
                                                      3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         0x00000005U /*!< 2 bits for pre-emption priority,
                                                      2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         0x00000004U /*!< 3 bits for pre-emption priority,
                                                      1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         0x00000003U /*!< 4 bits for pre-emption priority,
                                                      0 bit  for subpriority */

/********************************* Data structure *********************************/

/********************************* Global variables *******************************/
extern uint32_t g_uwTick;

/********************************* Inline prototypes ******************************/

/**
 * @brief  Provides the core clock count value.
 * @return core clock count value
 */
__attribute__((always_inline)) inline uint32_t GetSysCoreClockCount(void);

/**
 * @brief  Provides a tick value in millisecond.
 * @return tick value in millisecond
 */
__attribute__((always_inline)) inline uint32_t GetSysTick(void);

/********************************* Prototypes *************************************/

/**
 * @brief  System Initialization
 */
void SYSTEM_Initialize();


/********************************* Inline implementations *************************/
__attribute__((always_inline)) inline uint32_t GetSysCoreClockCount(void)
{
	return DWT->CYCCNT;
}

__attribute__((always_inline)) inline uint32_t GetSysTick(void)
{
  return g_uwTick; 
}

#ifdef __cplusplus
}
#endif

#endif /* INC_SYSTEM_H_ */
