/**
 * @file crc.c
 * @brief CRC interface
 * @details This file contains the interface for CRC operations.
 * 
 * @author Phuc VU
 * @date Nov 11, 2025
 */


/********************************* Includes ***************************************/
#include <stdio.h>
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

    uint32_t polynomial = 0x04C11DB7; // standard CRC-32 polynomial
    uint32_t init_value = 0x00000000; // Initial value for CRC

    LL_CRC_SetInputDataReverseMode(CRC, LL_CRC_INDATA_REVERSE_NONE);
    LL_CRC_SetOutputDataReverseMode(CRC, LL_CRC_OUTDATA_REVERSE_NONE);
    LL_CRC_SetPolynomialCoef(CRC, polynomial);
    LL_CRC_SetPolynomialSize(CRC, LL_CRC_POLYLENGTH_32B);
    LL_CRC_SetInitialData(CRC, init_value);
}

uint32_t CRC_Compute(uint8_t const * const pData, uint32_t length)
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

    // Reset CRC calculation unit
    CRC->CR |= CRC_CR_RESET;

    // Feed data to CRC calculator
    for (i = 0; i < length; i++)
    {
        LL_CRC_FeedData8(CRC, (uint8_t)(pData[i] & 0xFF));
        //CRC->DR = pData[i];
    }

    // Read computed CRC value   
    uint32_t crc_value = LL_CRC_ReadData32(CRC);
    
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


void CRC_Test(void)
{
    /* Simple test function for CRC computation */

    CRC_Initialize();

    uint8_t test_data[] = { 0x12, 0x34, 0x56, 0x78 };
    uint32_t expected_crc = 0x188e5750; // Expected CRC-32 value for test_data

    uint32_t computed_crc = CRC_Compute(test_data, sizeof(test_data)/sizeof(test_data[0]));

    if (computed_crc == expected_crc)
    {
        // Test passed
        printf("CRC test passed.\r\n");
    }
    else
    {
        // Test failed
        printf("CRC test failed.\r\n");
    }

    CRC_Deinitialize();
}

/* EOF */
