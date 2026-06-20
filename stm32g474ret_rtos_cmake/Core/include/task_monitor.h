#ifndef __TASK_MONITOR_H_
#define __TASK_MONITOR_H_


/********************************* Includes ***************************************/
#include <stdint.h>
#include <FreeRTOS.h>
#include <queue.h>

#ifdef __cplusplus
extern "C" {
#endif

/********************************* Constants definition ***************************/

/********************************* Macros definition ******************************/

/********************************* Types definition *******************************/

typedef struct
{
    TaskHandle_t* p_task_handle;
    uint8_t       task_id;
} TaskMonitor_param_t;

/********************************* Global variable ********************************/


/********************************* API function prototype *************************/

void MonitorTask(void* pv_parameters);


#ifdef __cplusplus
}
#endif

#endif /* __TASK_MONITOR_H_ */
