/**
 * @file usart1.c
 * @brief USART1 interface
 * 
 * @author Phuc VU
 * @date Jun 13, 2026
 */

/********************************* Includes ***************************************/

#include <stdio.h>
#include <stm32g4xx.h>
#include <stm32g4xx_ll_usart.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_bus.h>

#include "usart1.h"
#include "stream.h"

/********************************* Define & Macros ********************************/

#define ENALBE_INTERRUPT		1

#define USART1_INT_PRIORITY		8U

#define RX_BUFFER_SIZE			128U

/********************************* Local variables ********************************/

static uint8_t _rxBuf[RX_BUFFER_SIZE];
static Stream_t _rxStream;

/********************************* Local prototypes *******************************/

static inline void InitializeUSART(void);
static inline void InitializeGPIO(void);

/********************************* Implementations ********************************/

void USART1_Initialize(void)
{
    InitializeGPIO();
    InitializeUSART();

	STREAM_Initialize(&_rxStream, _rxBuf, RX_BUFFER_SIZE);
}

static inline void InitializeGPIO(void)
{
    /* !!! NOTE : ALTERNATE FUNCTION (AFx) find in doc DS12288 p.75 */

	/* Pin PC4/PC5 */

    volatile uint32_t tmpreg = READ_BIT(RCC->AHB2ENR, LL_AHB2_GRP1_PERIPH_GPIOC);
    if (tmpreg != LL_AHB2_GRP1_PERIPH_GPIOC)
	{
    	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
	}

	/* PC4     ------> USART1_TX */
	LL_GPIO_SetPinMode      (GPIOC, LL_GPIO_PIN_4, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_4, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed     (GPIOC, LL_GPIO_PIN_4, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinPull      (GPIOC, LL_GPIO_PIN_4, LL_GPIO_PULL_NO);
	LL_GPIO_SetAFPin_0_7    (GPIOC, LL_GPIO_PIN_4, LL_GPIO_AF_7);

	/* Finally lock pin configuration */
	//LL_GPIO_LockPin(GPIOC, LL_GPIO_PIN_4);

	// PC5     ------> USART1_RX
	LL_GPIO_SetPinMode      (GPIOC, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_5, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed     (GPIOC, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinPull      (GPIOC, LL_GPIO_PIN_5, LL_GPIO_PULL_NO);
	LL_GPIO_SetAFPin_0_7    (GPIOC, LL_GPIO_PIN_5, LL_GPIO_AF_7);

	/* Finally lock pin configuration */
	//LL_GPIO_LockPin(GPIOC, LL_GPIO_PIN_5);
}


static inline void InitializeUSART(void)
{
    /* UART parameters:
	 * USART1 @ 0x4001_3800
	 * bauds 115200
	 * 8 bits
	 * 1 stop
	 * parity none
	 * oversampling 16
	 * 
	 */

    /* SYSTEM parameters:
     * PCLK2_Frequency = 25 MHz
     */

	/* Reset & disable CR1.UE */
	USART1->CR1 = 0;

	/* Clock USART1 (RCC) */
	LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK2);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

	/* default: asynchronous */

	/* Oversampling: default 16 */

	/* default: 1 stop, 8 bits, no parity */

	/* p.1609 - baud 115200 -> 217 = 0xD9 */
	USART1->BRR = 0xD9;

	/* FIFO Enable */
	SET_BIT(USART1->CR1, USART_CR1_FIFOEN);

    
    /* Receive enable CR1.RE / Transmitter enable CR1.TE */
	LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_TX_RX);
	
#if (ENALBE_INTERRUPT == 1)	
	/* IDLE interrupt enable */
	SET_BIT(USART1->CR1, USART_CR1_IDLEIE);

	/* Set priority of Interrupt */
	NVIC_SetPriority(LPUART1_IRQn, USART1_INT_PRIORITY);

	/* Enable IRQ in NVIC */
	NVIC_EnableIRQ(USART1_IRQn);
#endif

    /* Enable CR1.UE */
	LL_USART_Enable(USART1);
}

void USART1_Write(uint8_t value)
{
	if (USART1->ISR & USART_ISR_TXE_TXFNF)
	{
		USART1->TDR = value;
	}
}


void USART1_IRQHandler(void)
{
	if (USART1->ISR & USART_ISR_IDLE)
	{
		/* Clear flag IDLE */
		SET_BIT(USART1->ICR, USART_ICR_IDLECF);

		/* Read all in buffer FIFO */
		uint8_t count = 0;
		uint8_t c = 0;
		while(USART1->ISR & USART_ISR_RXNE_RXFNE)
		{
			count++;
			c = USART1->RDR;
			(void)STREAM_Write(&_rxStream, &c, 1);
		}
	}
}
