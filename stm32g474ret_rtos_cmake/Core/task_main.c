/**
 * @file task_main.c
 * @brief Task main.
 * 
 * @author Phuc VU
 * @date Jun 18, 2026
 */

/********************************* Includes ***************************************/

#include "task_main.h"
#include "gpio.h"
#include "usart1.h"

/********************************* Constants definition ***************************/

/********************************* Macros definition ******************************/

/********************************* Types definition *******************************/

/********************************* Local variable *********************************/

/********************************* Local functions prototype **********************/

/********************************* API functions **********************************/

void MainTask(void* pv_parameters)
{
    (void)pv_parameters;
    //TaskMain_param_t* p_task_param = NULL;
    //p_task_param = (TaskMain_param_t*) pv_parameters;

    while(1)
    {
        vTaskDelay(1000);

        LL_GPIO_TogglePin(BUILT_IN_LED_Port, BUILT_IN_LED_Pin);

        #if 1
        uint8_t buf[3] = { 0x33, 0x22, 0x11 };
        USART1_Write(buf, 3);
        #endif
    }
}

/*EOF*/
