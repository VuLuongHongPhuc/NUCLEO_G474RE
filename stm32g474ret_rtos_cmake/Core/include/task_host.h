#ifndef __TASK_HOST_H_
#define __TASK_HOST_H_



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
} TaskHost_param_t;

/********************************* Global variable ********************************/

/********************************* API functions prototype ************************/

/**
 * @brief Connected to host 
 */
void HostTask(void* pv_parameters);


#ifdef __cplusplus
}
#endif

#endif /* __TASK_HOST_H_ */