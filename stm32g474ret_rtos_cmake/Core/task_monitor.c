/**
 * @file task_monitor.c
 * @brief Debug task monitor.
 * 
 * @author Phuc VU
 * @date Jun 18, 2026
 */

/********************************* Includes ***************************************/

#include <stdio.h>
#include <stm32g4xx_ll_lpuart.h>
#include "task_config.h"
#include "task_monitor.h"
#include "lpuart1.h"
#include "convert_to_string.h"


/********************************* Constants definition ***************************/

#define MONITOR_THREAD_STACK   1

/********************************* Macros definition ******************************/

/********************************* Types definition *******************************/

/********************************* Local variable *********************************/

/********************************* Local functions prototype **********************/

/********************************* API functions **********************************/

void MonitorTask(void* pParameters)
{    
#if (MONITOR_THREAD_STACK == 1)
    TaskMonitor_param_t* pTaskParam = NULL;

    TaskHandle_t taskHandle;
    UBaseType_t freeStack;
    uint8_t taskId = 0;
    size_t i;
    char str[configMAX_TASK_NAME_LEN + 1];
    char * pTaskName;
    bool bEndCaracter;
    
    pTaskParam = (TaskMonitor_param_t*) pParameters;
#endif
    

    while(1)
    {
        vTaskDelay(1000);

        
#if (MONITOR_THREAD_STACK == 1)
        taskHandle = pTaskParam->p_task_handle[taskId++];
        
        if (taskId == TSK_ID_MAX)
        {
            taskId = 0;
        }

        freeStack = uxTaskGetStackHighWaterMark(taskHandle);

        /* Task Name */
        bEndCaracter = false;
        pTaskName = pcTaskGetName(taskHandle);
        for(i=0; i<configMAX_TASK_NAME_LEN; i++)
        {
            if (bEndCaracter)
            {
                str[i] = 0x20; /* space */
            }
            else
            {
                str[i] = *pTaskName++;
            }
            
            if (str[i] == '\0')
            {
                str[i] = 0x20;
                bEndCaracter = true;
            }
        }
        LPUART1_Write((uint8_t*)str, configMAX_TASK_NAME_LEN);

        /* Stack free space left */
        (void)u32toa((uint32_t)freeStack, str);
        LPUART1_Write((uint8_t*)str, 11);
#endif
    }
}

/*EOF*/
