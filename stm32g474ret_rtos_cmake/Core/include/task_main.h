#ifndef __TASK_MAIN_H_
#define __TASK_MAIN_H_



#ifdef __cplusplus
extern "C" {
#endif

/********************************* Includes ***************************************/
#include <stdint.h>
#include <FreeRTOS.h>
#include <queue.h>

/********************************* Constants definition ***************************/
/********************************* Macros definition ******************************/
/********************************* Types definition *******************************/

typedef struct
{
    uint8_t      task_id;
} TaskMain_param_t;

/********************************* Global variable ********************************/
/********************************* API functions prototype ************************/

extern void MainTask(void* pv_parameters);


#ifdef __cplusplus
}
#endif

#endif /* __TASK_MAIN_H_ */