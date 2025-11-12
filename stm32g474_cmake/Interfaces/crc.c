/**
 * @file crc.c
 * @brief CRC interface
 * @details This file contains the interface for CRC operations.
 * 
 * @author Phuc VU
 * @date Nov 11, 2025
 */


/********************************* Includes ***************************************/
#include <stm32g474xx.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_crc.h>

#include "crc.h"
 
 
/********************************* Macros *****************************************/

/********************************* Data structure *********************************/

/********************************* Global variables *******************************/

/********************************* Prototypes *************************************/

/********************************* Implementations ********************************/

void CRC_Initialize(void)
{
    /*
    * CRC_IDR: CRC independent data register
    * CRC_DR: CRC data register -> input new data here to compute CRC
    * CRC_CR: CRC control register - REV_OUT
    * CRC_POL: polynomial register
    * CRC_INIT: initial value register
    */

    /*
    * CRC->CR reset value = 0x00000000
    * - REV_IN = 00: No bit reversal
    * - REV_OUT = 0: No bit reversal
    * - POLYSIZE = 00: 32-bit polynomial
    * - RESET = 0: No effect
    * - Default settings are suitable for standard CRC-32
    * 
    * CRC->POL reset value = 0x04C11DB7 (standard CRC-32 polynomial)
    * CRC->INIT reset value = 0xFFFFFFFF
    * CRC->IDR reset value = 0x00000000
    */

    /* Enable CRC clock */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC);

    CRC->INIT = 0x0;
    CRC->POL = 0x04C11DB7;
    CRC->CR &= ~CRC_CR_REV_OUT; // No bit reversal for output
    CRC->CR &= ~CRC_CR_REV_IN;  // No bit reversal for input
    CRC->CR |= CRC_CR_POLYSIZE_1; // 32-bit polynomial size
}

uint32_t CRC_Compute(uint32_t * pData, uint32_t length)
{
    /*
    * Input:
    * - data: pointer to data buffer
    * - length: number of 32-bit words in data buffer
    * 
    * Output:
    * - computed CRC value
    */

    uint32_t i;
    uint32_t crc_value;

    // Reset CRC calculation unit
    CRC->CR |= CRC_CR_RESET;

    // Feed data to CRC calculator
    for (i = 0; i < length; i++)
    {
        CRC->DR = pData[i];
    }

    // Get computed CRC value
    crc_value = CRC->DR;

    //crc_value ^= 0xFFFFFFFF; // Final XOR value for standard CRC-32

    return crc_value;
}


void CRC_Deinitialize(void)
{
    /*
    * De-initialize CRC peripheral
    * by disabling its clock to save power
    */

   /* Disable CRC clock */
   LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_CRC);
}


/* EOF */
