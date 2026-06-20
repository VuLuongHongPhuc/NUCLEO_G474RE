/*
 * stream.h
 *
 *  Created on: Dec 10, 2024
 *      Author: admin
 *
 * NOTE:
 *  - don't work if buffer size is under 3
 *  - data corrupted if overflow
 */

#ifndef UTIL_STREAM_H_
#define UTIL_STREAM_H_

/********************************* Includes ***************************************/
#include <stdint.h>
#include <stdbool.h>
//#include <stddef.h>

/********************************* Macros *****************************************/
#define STREAM_NO_ERROR            0x00U
#define STREAM_ERROR_OVERFLOW      0x01U
#define STREAM_ERROR_NULL_POINTER  0x02U


/********************************* Data structure *********************************/

/**
 * @struct Stream
 * @brief buffer object to store data stream
 */
typedef struct Stream
{
    uint16_t size;            /*!< Stream buffer size */
    uint8_t* buffer;         /*!< Stream buffer */
    uint8_t* end_of_stream;  /*!< Stream buffer end (first invalid address) */
    uint8_t* volatile read;  /*!< Read pointer */
    uint8_t* volatile write; /*!< Write pointer */
} Stream_t;


/********************************* Global variables *******************************/

/********************************* Inline prototypes ******************************/

/********************************* Prototypes *************************************/
/**
 * @brief Initialize a data stream
 *
 * @param pStream - Pointer stream to initialize
 * @param pBuffer - Pointer to buffer store the stream data
 * @param size - size of the buffer in bytes
 */
void STREAM_Initialize(Stream_t * pStream, uint8_t * pBuffer, uint16_t size);


void STREAM_Flush(Stream_t* stream);


bool STREAM_IsEmpty(Stream_t const * const pStream);


bool STREAM_Write(Stream_t * const pStream, const uint8_t* pBuf, uint16_t length);


uint16_t STREAM_Read(Stream_t * const pStream, uint8_t * pBuf, uint16_t length);


uint16_t STREAM_Look(Stream_t const * const pStream, uint16_t offset, uint8_t * const pBuf, uint16_t length);


uint16_t STREAM_Length(Stream_t const * const pStream);


#endif /* UTIL_STREAM_H_ */
