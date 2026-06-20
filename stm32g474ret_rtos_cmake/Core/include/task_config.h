/**
 * @file task_config.h
 * @brief Configuration definition for freeRTOS task
  * 
 * @author Phuc VU
 * @date Jun 18, 2026
 */

#ifndef __TASK_CONFIG_H_
#define __TASK_CONFIG_H_



#ifdef __cplusplus
extern "C" {
#endif

/********************************* Includes ***************************************/
#include <stdint.h>
#include "FreeRTOSConfig.h"

/********************************* Constants definition ***************************/

/* Tasks Stack size */
#define TSK_STACKSIZE_MAIN        (512)
#define TSK_STACKSIZE_HOST        (512)
#define TSK_STACKSIZE_MONITOR     (256)

/* Tasks ID */
#define TSK_ID_MAIN         0
#define TSK_ID_HOST         (TSK_ID_MAIN + 1)
#define TSK_ID_MONITOR      (TSK_ID_HOST + 1)
#define TSK_ID_MAX          (TSK_ID_MONITOR + 1)

/* Define priority configMAX_PRIORITIES = 7 */
#define PRIORITY_HIGHEST        (configMAX_PRIORITIES - 1)
#define PRIORITY_HIGH           (configMAX_PRIORITIES - 2)
#define PRIORITY_NORMAL         (configMAX_PRIORITIES - 3)
#define PRIORITY_LOW            (configMAX_PRIORITIES - 4)
#define PRIORITY_LOWEST         (configMAX_PRIORITIES - 5)
#define PRIORITY_ABOVE_IDLE     (configMAX_PRIORITIES - 6)
#define PRIORITY_IDLE           (0)

/* Tasks priority */
#define TSK_PRIORITY_MAIN            PRIORITY_NORMAL
#define TSK_PRIORITY_HOST            PRIORITY_LOW
#define TSK_PRIORITY_MONITOR         PRIORITY_ABOVE_IDLE
#define TSK_PRIORITY_IDLE            PRIORITY_IDLE         /* Idle task */

/* Tasks name for debug */
#define TSK_NAME_MAIN                 "TaskMain"
#define TSK_NAME_HOST                 "TaskHost"
#define TSK_NAME_MONITOR              "TaskMonitor"


/********************************* Macros definition ******************************/
/********************************* Types definition *******************************/
/********************************* Global variable ********************************/
/********************************* API functions prototype ************************/



#ifdef __cplusplus
}
#endif

#endif /* __TASK_CONFIG_H_ */