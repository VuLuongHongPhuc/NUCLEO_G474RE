/**
 * @file task_monitor.c
 * @brief Debug task monitor.
 * 
 * @author Phuc VU
 * @date Jun 18, 2026
 */

/********************************* Includes ***************************************/

#include <stdio.h>
#include "task_config.h"
#include "task_monitor.h"
#include "lpuart1.h"
#include "convert_to_string.h"

/********************************* Constants definition ***************************/

/********************************* Macros definition ******************************/

/********************************* Types definition *******************************/

/********************************* Local variable *********************************/

/********************************* Local functions prototype **********************/

/********************************* API functions **********************************/





void MonitorTask(void* pv_parameters)
{    
    TaskMonitor_param_t* p_task_param = NULL;
    p_task_param = (TaskMonitor_param_t*) pv_parameters;

    TaskHandle_t task_handle;
    UBaseType_t free_stack;
    uint8_t task_id = 0;
    
    //char str[11];


    printf("Task Name\t\t");
    printf("Free stack space in byte\n");
    printf("-----------------------------------------\n");
    while(1)
    {
        vTaskDelay(1000);

        task_handle = p_task_param->p_task_handle[task_id++];
        
        if (task_id == TSK_ID_MAX)
        {
            task_id = 0;
        }

        free_stack = uxTaskGetStackHighWaterMark(task_handle);
        //if (free_stack < 20)
        {
            printf("%s\t\t", pcTaskGetName(task_handle));
            printf("%u\n", (unsigned int)free_stack);

            //(void)u32toa((uint32_t)free_stack, str);
            //printf("%s\n", str);
        }
            

        #if 0
        LPUART1_TestTransmit();
        LPUART1_TestReceive();
        #endif
    }
}

/*EOF*/
