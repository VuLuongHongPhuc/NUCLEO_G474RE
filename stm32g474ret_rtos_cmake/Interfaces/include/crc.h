/**
 * @file crc.h
 * @brief CRC interface
 * @details This file contains the interface for CRC operations.
 * 
 * @author Phuc VU
 * @date Nov 11, 2025
 */

#ifndef DRV_CRC_H_
#define DRV_CRC_H_

/********************************* Includes ***************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/********************************* Macros *****************************************/

/********************************* Data structure *********************************/

/********************************* Global variables *******************************/

/********************************* Prototypes *************************************/

/**
 * @brief Initialize the CRC peripheral
 */
void CRC_Initialize(void);

uint32_t CRC_Compute(uint8_t const * const pData, uint32_t length);

void CRC_Deinitialize(void);

void CRC_Test(void);

#ifdef __cplusplus
}
#endif

#endif /* DRV_CRC_H_ */
