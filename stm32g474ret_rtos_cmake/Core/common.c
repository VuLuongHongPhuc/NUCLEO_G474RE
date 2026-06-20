/**
 * @file common.c
 * @brief FreeRTOS function to implement
  * 
 * @author Phuc VU
 * @date Jun 18, 2026
 */

/********************************* Includes ***************************************/
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>


/********************************* Constants definition ***************************/

/********************************* Macros definition ******************************/

/********************************* Types definition *******************************/

/********************************* Local variable *********************************/

/********************************* Local functions prototype **********************/

/********************************* Extern functions prototype *********************/
extern void Error_Handler(void);

/********************************* API functions **********************************/


/**
 * @brief Infinite loop called in case of stack overflow detection, for debug purposes
 * 
 * @param xTask Handle to the task were the stack overflow was detected
 * @param pcTaskName Handle to the task name (might be correupted due to the stack overflow)
 * 
*/
void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName)
{
    (void) xTask;
    
    printf("Stack overflow detected in task: %s\n", pcTaskName);

    Error_Handler();
}

/*EOF*/
