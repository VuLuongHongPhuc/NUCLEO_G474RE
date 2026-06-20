/**
 * @file task_host.c
 * @brief Task host to communicate with PC.
 * 
 * @author Phuc VU
 * @date Jun 18, 2026
 */

/********************************* Includes ***************************************/

#include "task_host.h"

/********************************* Constants definition ***************************/

/********************************* Macros definition ******************************/

/********************************* Types definition *******************************/

/********************************* Local variable *********************************/

/********************************* Local functions prototype **********************/

/********************************* API functions **********************************/

void HostTask(void* pv_parameters)
{
    (void)pv_parameters;
    //TaskHost_param_t* p_task_param = NULL;
    //p_task_param = (TaskHost_param_t*) pv_parameters;


    while (1)
    {
        /* code */
        vTaskDelay(1000);

        
    }
    
}

/*EOF*/
