/*
 * console.c
 *
 *  Created on: Dec 12, 2024
 *      Author: admin
 */


#include "stm32g474xx.h"
#include "console.h"

#define MAX_STRING_SIZE		128

static void uli2a(unsigned long int num, unsigned int base, int uc,char * bf);

void CONSOLE_Write(const char* ptr)
{
	uint8_t i;

	for (i=0; i<MAX_STRING_SIZE; i++)
	{
		// if end string
		if (*ptr == 0)
			break;

		ITM_SendChar(*ptr++);
	}
}

void CONSOLE_WriteValue(uint32_t value, uint8_t base)
{
	uint8_t i;
	uint8_t uc = 0;
	char str[12] = {0};

	if (base == 16)
	{
		ITM_SendChar('0');
		ITM_SendChar('x');
		uc = 1;
	}

	uli2a(value, base, uc, str);

	for(i=0; i<12; i++)
	{
		if (str[i] == 0)
			break;

		ITM_SendChar(str[i]);
	}

	ITM_SendChar('\r');
	ITM_SendChar('\n');
}

// tyniprintf
static void uli2a(unsigned long int num, unsigned int base, int uc,char * bf)
{
	int n=0;
	unsigned int d=1;

	while (num/d >= base)
		d*=base;

	while (d!=0)
	{
		int dgt = (int)(num / d);
		num%=d;
		d/=base;
		if (n || dgt>0|| d==0)
		{
			*bf++ = (char)(dgt+(dgt<10 ? '0' : (uc ? 'A' : 'a')-10));
			++n;
		}
	}
	*bf=0;
}
/*EOF*/
