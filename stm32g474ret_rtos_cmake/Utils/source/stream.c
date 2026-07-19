/*
 * stream.c
 *
 *  Created on: Dec 10, 2024
 *      Author: Phuc VU
 */


/********************************* Includes ***************************************/

#include "stream.h"

/********************************* Constants definition ***************************/

/********************************* Macros definition ******************************/

/********************************* Types definition *******************************/

/********************************* Private variable *******************************/

static uint8_t _errorCode;

/********************************* API functions **********************************/

uint8_t STREAM_GetLastError(void)
{
    return _errorCode;
}

/**
 * @brief Initialize a data stream
 *
 * @param pStream - Pointer stream to initialize
 * @param pBuffer - Pointer to buffer store the stream data
 * @param size - size of the buffer in bytes
 */
void STREAM_Initialize(Stream_t * pStream, uint8_t * pBuffer, uint16_t size)
{
    if(!pStream || !pBuffer)
    {
        _errorCode = STREAM_ERROR_NULL_POINTER;
        return;
    }

    _errorCode = STREAM_NO_ERROR;

    /* Initialize the stream object */
    pStream->size = size;
    pStream->buffer = pBuffer;
    pStream->end_of_stream = &pBuffer[size];
    pStream->read = pBuffer;
    pStream->write = pBuffer;
}

void STREAM_Flush(Stream_t * pStream)
{
    if(!pStream)
    {
        _errorCode = STREAM_ERROR_NULL_POINTER;
        return;
    }

    pStream->read = pStream->buffer;
    pStream->write = pStream->buffer;
}

bool STREAM_IsEmpty(Stream_t const * const pStream)
{
    if(!pStream)
    {
        _errorCode = STREAM_ERROR_NULL_POINTER;
        return false;
    }

    if( pStream->write == pStream->read )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool STREAM_Write(Stream_t * const pStream, const uint8_t* pBuf, uint16_t length)
{
    if(!pStream || !pBuf)
    {
        _errorCode = STREAM_ERROR_NULL_POINTER;
        return false;
    }

    while (length!=0)
    {
        /* Copy current byte */
        *pStream->write = *pBuf;

        /* Next byte */
        pStream->write++;
        pBuf++;
        length--;

        /* Check the end of pStream */
        if (pStream->write==pStream->end_of_stream)
        {
            pStream->write = pStream->buffer;
        }

        /* Check pStream overflow */
        if (pStream->write == pStream->read)
        {
            /* Move ahead the read pointer */
            pStream->read++;
            if( pStream->read==pStream->end_of_stream )
            {
                pStream->read = pStream->buffer;
            }

            _errorCode = STREAM_ERROR_OVERFLOW;
        }
    }

    return true;
}

uint16_t STREAM_Read(Stream_t * const pStream, uint8_t * pBuf, uint16_t length)
{
    if(!pStream || !pBuf)
    {
        _errorCode = STREAM_ERROR_NULL_POINTER;
        return 0;
    }

    uint16_t count = 0;
    while ((length != 0) && (pStream->read != pStream->write))
    {
        /* Read current byte */
        *pBuf = *pStream->read;
        count++;

        /* Next byte */
        pStream->read++;
        pBuf++;
        length--;

        /* Check the end of pStream */
        if (pStream->read == pStream->end_of_stream)
        {
            pStream->read = pStream->buffer;
        }
    }

    return count;
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
uint16_t STREAM_Look(Stream_t const * const pStream, uint16_t offset, uint8_t * const pBuf, uint16_t length)
{
    if (!pStream || !pBuf)
    {
        _errorCode = STREAM_ERROR_NULL_POINTER;
        return 0;
    }

    const uint16_t stream_data_size = STREAM_Length(pStream);
    uint16_t absolute_offset;
    uint16_t count = 0;
    while ((count < length) && (count < (stream_data_size - offset)))
    {
        /* Read byte */
        absolute_offset = (uint16_t)(pStream->read - pStream->buffer + offset + count);
        pBuf[count]   = pStream->buffer[absolute_offset % pStream->size];
        count++;
    }

    return count;
}

/**
 * @brief Get the length of the data inside the stream buffer
 *
 * @param stream stream to read from
 *
 * @return the length of the data
 */
uint16_t STREAM_Length(Stream_t const * const pStream)
{
    return pStream->write >= pStream->read
             ? (uint16_t)(pStream->write - pStream->read)
             : (uint16_t)(pStream->write + pStream->size - pStream->read);
}



/*EOF*/
