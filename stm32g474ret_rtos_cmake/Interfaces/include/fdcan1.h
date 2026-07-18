/**
 * @file can.h
 * @brief CAN Interface
 * 
 * @author Phuc VU
 * @date Nov 13, 2025
 */

#ifndef __FDCAN1_H_
#define __FDCAN_H_

/* p.44 table of content / p.1938 FDCAN */

/********************************* Includes ***************************************/

#include "fdcan_common.h"


#ifdef __cplusplus
extern "C" {
#endif

/********************************* Constants definition ***************************/

/********************************* Macros definition ******************************/

/********************************* Types definition *******************************/

/********************************* Public variable ********************************/

/********************************* API functions prototype ************************/

void FDCAN1_Initialize(void);

bool FDCAN1_GetMessage(FDCAN_RxHeaderTypeDef * pRxHeader, uint8_t * pRxData);

#ifdef __cplusplus
}
#endif


#endif /* __FDCAN1_H_ */