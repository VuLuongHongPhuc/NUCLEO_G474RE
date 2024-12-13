/*
 * system.h
 *
 *  Created on: May 12, 2024
 *      Author: admin
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_


#define BOARD_SYSTEM_CORE_CLOCK          (100000000)     /* 100 Mhz */


/** @defgroup CORTEX_Preemption_Priority_Group CORTEX Preemption Priority Group
  * @{
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




void SYSTEM_Initialize();


uint32_t GetSysTick();
uint32_t GetSysTickPriority();
uint32_t GetSysCoreClockCount();

#endif /* INC_SYSTEM_H_ */
