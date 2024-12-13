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

#ifndef BSP_STREAM_H_
#define BSP_STREAM_H_


#include <stdint.h>


/******************** TYPE DEFINITION ****************************************/
/** @brief Stream object */
typedef struct Stream
{
    /** @brief Stream buffer size */
    uint8_t size;

    /** @brief Stream buffer */
    uint8_t* buffer;

    /** @brief Stream buffer end (first invalid address) */
    uint8_t* end_of_stream;

    /** @brief Read pointer */
    uint8_t* volatile read;

    /** @brief Write pointer */
    uint8_t* volatile write;

} Stream_t;



/******************** API FUNCTION PROTOTYPE *********************************/
/** @brief Initialize a data stream */
void STREAM_Init(Stream_t* stream, uint8_t* buffer, uint8_t size);

/** @brief Flush a data stream */
void STREAM_Flush(Stream_t* stream);

/** @brief Check if a stream is empty */
uint8_t STREAM_IsEmpty(Stream_t* stream);

/** @brief Write in a data stream */
void STREAM_Write(Stream_t* stream, const uint8_t* buffer, uint8_t length);

/** @brief Read data from a data stream */
void STREAM_Read(Stream_t* stream, uint8_t* buffer, uint8_t length, uint8_t* read);

/** @brief Look at data from a data stream */
void STREAM_Look(const Stream_t* const stream,
				const uint8_t            offset,
				uint8_t* const           buffer,
				const uint8_t            length,
				uint8_t* const           read);

/** @brief Get the length of the data stream */
uint8_t STREAM_Length(const Stream_t* const stream);


#endif /* BSP_STREAM_H_ */
