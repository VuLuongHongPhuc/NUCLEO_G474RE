/*
 * console.h
 *
 *  Created on: Dec 12, 2024
 *      Author: admin
 */

#ifndef UTILITY_CONSOLE_H_
#define UTILITY_CONSOLE_H_

#include <stdint.h>

#ifdef DEBUG
	#define DEBUG_Write(ptr)			CONSOLE_Write(ptr)
	#define DEBUG_WriteValue(x, base)		CONSOLE_WriteValue(x, base)
#else
	#define DEBUG_Write(ptr)
#endif


void CONSOLE_Write(const char* ptr);
void CONSOLE_WriteValue(uint32_t value, uint8_t base);

#endif /* UTILITY_CONSOLE_H_ */
