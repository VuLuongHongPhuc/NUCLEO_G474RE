/*
 * task_cli.c
 *
 *  Created on: Dec 10, 2024
 *      Author: admin
 */

#include <stm32g4xx_ll_gpio.h>

#include "cli_task.h"
#include "stream.h"
#include "gpio.h"
#include "lpuart1.h"


/*** LOCAL DEFINE *****************************************************/
const char TxtTerminal[] = "Host>";
const char TxtDirectiveGpio[] = "gpio";
const char TxtWelcome[] = "STM32G474RE 128k RAM 512k FLASH\r\n";

#define REQUEST_DATA_SIZE	128

#define NULL ((void*)0)

/*** LOCAL TYPE *******************************************************/
typedef enum {
	STATE_IDLE = 0,
	STATE_ONGOING,
	STATE_RELEASE
}CLI_STATE;

/*** LOCAL VARIABLES **************************************************/
static CLI_STATE state;
static uint8_t request[REQUEST_DATA_SIZE];

Stream_t* pRxStream = 0;

/*** LOCAL PROTOTYPE **************************************************/
static void Idle(void);
static void ExecuteRequest(void);
static void Release(void);
static void ExecuteGPIO(const char *str);
static void ToggleGPIO(const char* str);
static char* FindSubstring(const char *str, const char *sub);


void CLI_Initialize(void)
{
	pRxStream = LPUART1_GetRxStream();

	// welcome text
	LPUART1_Write((const uint8_t*)TxtWelcome, sizeof(TxtWelcome));

	Release();
}

void CLI_task(void)
{

	switch(state)
	{
	case STATE_IDLE:
		// Wait request
		Idle();
		break;

	case STATE_ONGOING:
		// Execute request
		ExecuteRequest();
		break;

	case STATE_RELEASE:
		// Give back hand to host
		Release();
		break;

	default:
		break;
	}
}

/**
 * @brief Put in buffer all terminal input
 */
static void Idle(void)
{
	static uint8_t index = 0;
	uint8_t c;
	uint8_t read;

	STREAM_Read(pRxStream, &c, 1, &read);

	if (read)
	{
		// BS (Back Space) = 0x8
		if (c == 0x8)
		{
			if (index)
			{
				index--;
				LPUART1_Write(&c, 1);
			}
		}
		else
		{
			request[index++] = c;
			LPUART1_Write(&c, 1);

			// 0xd = CR, 0xa = LF
			if (c == 0xd)
			{
				// execute request
				state = STATE_ONGOING;

				// reset index
				index = 0;
			}
		}
	}
}

static void ExecuteRequest(void)
{
	uint8_t i;

	// retrieve the request
	for (i=0; i<REQUEST_DATA_SIZE; i++)
	{
		if (request[i] == 0xd)
		{
			// replace --> end string
			request[i] = 0;
			break;
		}
	}

	// find directive
	if (FindSubstring((const char*)request, TxtDirectiveGpio) == (char*)request)
	{
		// example : Host>gpio t 1
		ExecuteGPIO((const char*)&request[sizeof(TxtDirectiveGpio)]);
	}
	else
	{
		char txt[] = "\r\n";
		LPUART1_Write((const uint8_t*)txt, sizeof(txt));
	}

	state = STATE_RELEASE;
}

static void Release(void)
{
	LPUART1_Write((const uint8_t*)TxtTerminal, sizeof(TxtTerminal));
	state = STATE_IDLE;
}

static void ExecuteGPIO(const char* str)
{
	switch(str[0])
	{
	case 'r':
		break;

	case 'w':
		break;

	case 't':
		{
			char txt[] = "\r\nToggle\r\n";
			LPUART1_Write((const uint8_t*)txt, sizeof(txt));
			ToggleGPIO(&str[1]);
		}break;

	default:
		break;
	}
}

static void ToggleGPIO(const char* str)
{
	// if not space character
	if (str[0] != 0x20)
		return;

	switch(str[1])
	{
	case '1':
		LL_GPIO_TogglePin(BUILT_IN_LED_Port, BUILT_IN_LED_Pin);

		char txt[] = "BUILT_IN_LED\r\n";
		LPUART1_Write((const uint8_t*)txt, sizeof(txt));
		break;

	default:
		break;
	}
}

// Function to find a substring within a string
static char* FindSubstring(const char *str, const char *sub)
{
    const char *p1 = str, *p2, *p3;
    if (!*sub)
    {
        return (char *)str;
    }
    while (*p1)
    {
        p2 = p1;
        p3 = sub;
        while (*p3 && (*p2 == *p3))
        {
            p2++;
            p3++;
        }
        if (!*p3)
        {
            return (char *)p1;
        }
        p1++;
    }
    return NULL;
}
/*EOF*/
