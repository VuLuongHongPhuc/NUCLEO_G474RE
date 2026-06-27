

/*
 * STM32Cube_FW_G4_V1.5.1
 * C:\Users\xxx\STM32Cube\Repository\STM32Cube_FW_G4_V1.5.1\Drivers\STM32G4xx_HAL_Driver\Src
 * C:\Users\xxx\STM32Cube\Repository\STM32Cube_FW_G4_V1.5.1\Drivers\STM32G4xx_HAL_Driver\Inc
 * */


/********************************* Includes ***************************************/
#include <stdio.h>

#include <stm32g474xx.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_iwdg.h>
//#include <stm32g4xx_ll_bus.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include "main.h"
#include "task_config.h"
#include "task_main.h"
#include "task_host.h"
#include "task_monitor.h"
#include "system.h"
#include "lpuart1.h"
#include "watchdog.h"
#include "gpio.h"
#include "usart1.h"


/********************************* Constants definition ***************************/

/********************************* Macros definition ******************************/

/********************************* Types definition *******************************/

/********************************* Local variable *********************************/

/* Task param */
static TaskMain_param_t _param_task_main = {0};
static TaskHost_param_t _param_task_host = {0};
static TaskMonitor_param_t _param_task_monitor = {0};

static TaskHandle_t _taskHandle[TSK_ID_MAX] = {0};

/********************************* Local functions prototype **********************/

static void CreateTask(void);

/********************************* API functions **********************************/


int main(void)
{
	LL_RCC_ClocksTypeDef clock_ref = {0};
	
    SYSTEM_Initialize();

	LL_RCC_GetSystemClocksFreq(&clock_ref);/* Get clock */
	GPIO_Initialize();	
	USART1_Initialize(&clock_ref.PCLK2_Frequency);
    LPUART1_Initialize(clock_ref.PCLK1_Frequency);
	//WATCHDOG_Initialize();

	
	/* DEBUG - clocks frequencies */
	printf("SYSTEM frequencies:\r\n");
	printf(" - SYSCLK: %d\r\n", (int)clock_ref.SYSCLK_Frequency);
	printf(" - HCLK  : %d\r\n", (int)clock_ref.HCLK_Frequency);
	printf(" - PCLK1 : %d\r\n", (int)clock_ref.PCLK1_Frequency);
	printf(" - PCLK2 : %d\r\n", (int)clock_ref.PCLK2_Frequency);


	CreateTask();	

    vTaskStartScheduler();

	Error_Handler();

	return 0;
}

static void CreateTask(void)
{
	/* Create task main */
    static StaticTask_t xMainTaskBuffer __attribute__((section(".ccmsram.os.task.main")));    
    static StackType_t xMainTaskStack[TSK_STACKSIZE_MAIN] __attribute__((section(".ccmsram.os.stack.main")));
    /* Prepare parameters */
    _param_task_main.task_id = TSK_ID_MAIN;
	
	_taskHandle[TSK_ID_MAIN] = xTaskCreateStatic( MainTask,         /* Function that implements the task. */
                                (const char * const) TSK_NAME_MAIN, /* Text name for the task. */
                                TSK_STACKSIZE_MAIN,                 /* Number of indexes in the xStack array. */
                                (void * const) &_param_task_main,    /* Parameter passed into the task. */
                                TSK_PRIORITY_MAIN,                  /* Priority at which the task is created. */
                                xMainTaskStack,                     /* Array to use as the task's stack. */
                                &xMainTaskBuffer);                  /* Variable to hold the task's data structure. */
	if (_taskHandle[TSK_ID_MAIN] == NULL)
    {
		/* Failed to create task */
        Error_Handler();
	}


	/* Create task host */
    static StaticTask_t xHostTaskBuffer __attribute__((section(".ccmsram.os.task.host")));     
    static StackType_t xHostTaskStack[TSK_STACKSIZE_HOST] __attribute__((section(".ccmsram.os.stack.host")));
    /* Prepare parameters */
    _param_task_host.task_id = TSK_ID_HOST;

	_taskHandle[TSK_ID_HOST] = xTaskCreateStatic( HostTask,         /* Function that implements the task. */
                                (const char * const) TSK_NAME_HOST, /* Text name for the task. */
                                TSK_STACKSIZE_HOST,                 /* Number of indexes in the xStack array. */
                                (void * const) &_param_task_host,    /* Parameter passed into the task. */
                                TSK_PRIORITY_HOST,                  /* Priority at which the task is created. */
                                xHostTaskStack,                     /* Array to use as the task's stack. */
                                &xHostTaskBuffer);                  /* Variable to hold the task's data structure. */
	if (_taskHandle[TSK_ID_HOST] == NULL)
    {
        Error_Handler();
    }


	/* Create task monitor - last create ! */
    static StaticTask_t xMonitorTaskBuffer __attribute__((section(".ccmsram.os.task.monitor")));
    static StackType_t xMonitorTaskStack[TSK_STACKSIZE_MONITOR] __attribute__((section(".ccmsram.os.stack.monitor")));
    /* Prepare parameters */
    _param_task_monitor.task_id = TSK_ID_MONITOR;
	_param_task_monitor.p_task_handle = _taskHandle;

	_taskHandle[TSK_ID_MONITOR] = xTaskCreateStatic( MonitorTask,         /* Function that implements the task. */
                                (const char * const) TSK_NAME_MONITOR, /* Text name for the task. */
                                TSK_STACKSIZE_MONITOR,                 /* Number of indexes in the xStack array. */
                                (void * const) &_param_task_monitor,    /* Parameter passed into the task. */
                                TSK_PRIORITY_MONITOR,                  /* Priority at which the task is created. */
                                xMonitorTaskStack,                     /* Array to use as the task's stack. */
                                &xMonitorTaskBuffer);                  /* Variable to hold the task's data structure. */
	if (_taskHandle[TSK_ID_MONITOR] == NULL)
    {
        Error_Handler();
    }
}


void Error_Handler(void)
{
	/* User can add his own implementation to report the HAL error return state */

	__disable_irq();
	while(1);
}



/*EOF*/
