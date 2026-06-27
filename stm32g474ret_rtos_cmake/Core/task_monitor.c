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
#include <stream_buffer.h>

/********************************* Constants definition ***************************/

/********************************* Macros definition ******************************/

/********************************* Types definition *******************************/

/********************************* Local variable *********************************/

/********************************* Local functions prototype **********************/

/********************************* API functions **********************************/

void MonitorTask(void* pv_parameters)
{    
    TaskMonitor_param_t* p_task_param = NULL;
    TaskHandle_t task_handle;
    UBaseType_t free_stack;
    uint8_t task_id = 0;

    size_t numberOfBytesSend, i;
    
    p_task_param = (TaskMonitor_param_t*) pv_parameters;
    

    char str[configMAX_TASK_NAME_LEN + 1];
    char * p_task_name;
    bool end_car;


    while(1)
    {
        vTaskDelay(1000);

        task_handle = p_task_param->p_task_handle[task_id++];
        
        if (task_id == TSK_ID_MAX)
        {
            task_id = 0;
        }

        free_stack = uxTaskGetStackHighWaterMark(task_handle);

        /* Task Name */
        end_car = false;
        p_task_name = pcTaskGetName(task_handle);
        for(i=0; i<configMAX_TASK_NAME_LEN; i++)
        {
            if (end_car)
            {
                str[i] = 0x20; /* space */
            }
            else
            {
                str[i] = *p_task_name++;
            }
            
            if (str[i] == '\0')
            {
                str[i] = 0x20;
                end_car = true;
            }
        }
        LPUART1_Write(str, configMAX_TASK_NAME_LEN);

        /* Stack free space left */
        (void)u32toa((uint32_t)free_stack, str);
        LPUART1_Write(str, 11);
    }
}

/*EOF*/
