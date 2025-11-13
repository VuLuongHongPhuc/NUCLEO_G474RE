/**
 * @file can.h
 * @brief CAN Interface
 * 
 * @author Phuc VU
 * @date Nov 13, 2025
 */

#ifndef DRV_CAN_H_
#define DRV_CAN_H_

/* p.44 table of content / p.1938 FDCAN */

/********************************* Includes ***************************************/

#ifdef __cplusplus
extern "C" {
#endif

/********************************* Macros *****************************************/
 
/********************************* Prototypes *************************************/

void CAN_Initialize(void);

#ifdef __cplusplus
}
#endif


#endif /* DRV_CAN_H_ */