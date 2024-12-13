/*
 * stream.c
 *
 *  Created on: Dec 10, 2024
 *      Author: admin
 */

#include "stream.h"



/******************** API FUNCTIONS ******************************************/
/**
* @brief Initialize a data stream
*
* @param stream - stream to initialize
* @param buffer - buffer to store the stream data
* @param size - size of the buffer in bytes
**/
void STREAM_Init(Stream_t* stream, uint8_t* buffer, uint8_t size)
{
    /* Check parameters */
    /* Null stream */
    if(stream == 0)
    {
        return;
    }
    /* Null buffer */
    if(buffer == 0)
    {
        return;
    }

    /* Initialize the stream object */
    stream->size = size;
    stream->buffer = buffer;
    stream->end_of_stream = &buffer[size];
    stream->read = buffer;
    stream->write = buffer;
}

/**
* @brief Flush a data stream
*
* @param stream stream to flush
**/
void STREAM_Flush(Stream_t* stream)
{
    /* Check parameters */
    /* Null stream */
    if(stream == 0)
    {
        return;
    }

    stream->read = stream->buffer;
    stream->write = stream->buffer;
}

/**
* @brief Check if a stream is empty
*
* @param stream stream to check
*
* @return 1 if the stream is empty, else 0
*
*/
uint8_t STREAM_IsEmpty(Stream_t* stream)
{
    /* Check parameters */
    /* Null stream */
    if(stream == 0)
    {
        return 0xFF;
    }

    if( stream->write==stream->read )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/**
* @brief Write in a data stream
*
* @param stream stream to write to
* @param buffer pointer to the data to write
* @param length length of the data to write in bytes
*
*/
void STREAM_Write(Stream_t* stream, const uint8_t* buffer, uint8_t length)
{
    /* Check parameters */
    /* Null stream */
    if(stream == 0)
    {
        return;
    }
    /* Null buffer */
    if(buffer == 0)
    {
        return;
    }

    while(length!=0)
    {
        /* Copy current byte */
        *stream->write = *buffer;

        /* Next byte */
        stream->write++;
        buffer++;
        length--;

        /* Check the end of stream */
        if( stream->write==stream->end_of_stream )
        {
            stream->write = stream->buffer;
        }

        /* Check stream overflow */
        if( stream->write==stream->read )
        {
            stream->read++;
            if( stream->read==stream->end_of_stream )
            {
                stream->read = stream->buffer;
            }

            // TODO: set flag overflow
        }
    }
}

/**
* @brief Read data stream
*
* @param[in]  stream - stream to read from
* @param[out] buffer - pointer to the buffer
* @param[in]  length - length of the buffer to write read data to
* @param[out] read   - Length of the data that was read
*
*/
void STREAM_Read(Stream_t* stream, uint8_t* buffer, uint8_t length, uint8_t* read)
{
    *read = 0;

    /* Check parameters */
    /* Null stream */
    if(stream == 0)
    {
        return;
    }
    /* Null buffer */
    if(buffer == 0)
    {
        return;
    }

    while(length!=0 && (stream->read!=stream->write))
    {
        /* Read current byte */
        *buffer = *stream->read;
        (*read)++;

        /* Next byte */
        stream->read++;
        buffer++;
        length--;

        /* Check the end of stream */
        if( stream->read==stream->end_of_stream )
        {
            stream->read = stream->buffer;
        }
    }
}

/**
 * @brief Look at data stream without consuming.
 *
 * @param[in]  stream - stream to read from
 * @param[in]  offset - the index at which to start reading
 * @param[out] buffer - pointer to the buffer
 * @param[in]  length - length of the buffer to write read data to
 * @param[out] read   - Length of the data that was read
 */
void STREAM_Look(const Stream_t* const stream,
				const uint8_t            offset,
				uint8_t* const           buffer,
				const uint8_t            length,
				uint8_t* const           read)
{
    *read = 0;

    /* Check parameters */
    /* Null stream */
    if (stream == 0)
    {
        return;
    }
    /* Null buffer */
    if (buffer == 0)
    {
        return;
    }

    const uint8_t stream_data_size = STREAM_Length(stream);

    uint8_t absolute_offset;

    while (*read < length && *read < (stream_data_size - offset))
    {
        /* Read byte */
        absolute_offset = (uint8_t)(stream->read - stream->buffer + offset + *read);
        buffer[*read]   = stream->buffer[absolute_offset % stream->size];
        (*read)++;
    }
}

/**
 * @brief Get the length of the data inside the stream buffer
 *
 * @param stream stream to read from
 *
 * @return the length of the data
 */
uint8_t STREAM_Length(const Stream_t* const stream)
{
    return stream->write >= stream->read
             ? (uint8_t)(stream->write - stream->read)
             : (uint8_t)(stream->write + stream->size - stream->read);
}



/*EOF*/
