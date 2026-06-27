/**
 * @file usart1.c
 * @brief USART1 interface
 * 
 * @author Phuc VU
 * @date Jun 13, 2026
 */

/********************************* Includes ***************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stm32g4xx.h>
#include <stm32g4xx_ll_usart.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_dma.h>

//#include "usart1.h"
#include "stream.h"

/********************************* Define & Macros ********************************/

#define USART_FIFO_SIZE         8U

#define USART1_INT_PRIORITY		8U
#define DMA1_CH1_INT_PRIORITY   9U

#define RX_BUFFER_SIZE			32U
#define TX_BUFFER_SIZE			128U

/********************************* Local variables ********************************/

static uint32_t _periphClock;

static uint8_t _txBuf[TX_BUFFER_SIZE+1];
static Stream_t _txStream;

static uint8_t _rxDmaBuf[RX_BUFFER_SIZE+1];

/********************************* Local prototypes *******************************/

static inline void InitializeUSART(void);
static inline void InitializeGPIO(void);
static inline void InitializeDMA(void);
static inline void InitializeInterrupt(void);
static inline void TransmitRemainingData(void);

/********************************* Implementations ********************************/

void USART1_Initialize(const uint32_t * const pclk2_frequency)
{
	_periphClock = *pclk2_frequency;

    InitializeGPIO();
    InitializeUSART();

	STREAM_Initialize(&_txStream, _txBuf, TX_BUFFER_SIZE);
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
	 * USART1 @ 0x4001_3800 p.82
	 * bauds 115200
	 * 8 bits
	 * 1 stop
	 * parity none
	 * oversampling 16
	 * FIFO ON
	 */

    /* SYSTEM parameters:
     * PCLK2_Frequency = 25 MHz
     */

	/* Disable CR1.UE */
	LL_USART_Disable(USART1);

	/* Clock USART1 (RCC) */
	LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK2);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

	/* default: asynchronous */

	/* Oversampling CR1.OVER8 (default 16) */
	//LL_USART_SetOverSampling(USART1, LL_USART_OVERSAMPLING_16);

	/* default: 1 stop, 8 bits, no parity */
	//LL_USART_ConfigCharacter(USART1, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

	/* p.1609 - BRR baud 115200 -> 217 = 0xD9 */
	LL_USART_SetBaudRate(USART1,
						_periphClock,
						LL_USART_GetPrescaler(USART1),
						LL_USART_OVERSAMPLING_16,
						115200);

	/* CR1.FIFOEN */
	LL_USART_EnableFIFO(USART1);

    /* Receive enable CR1.RE / Transmitter enable CR1.TE */
	LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_TX_RX);

	
	InitializeInterrupt();

	InitializeDMA();

    /* Enable CR1.UE */
	LL_USART_Enable(USART1);
}

static inline void InitializeInterrupt(void)
{
	NVIC_SetPriority(USART1_IRQn, USART1_INT_PRIORITY);	/* Set interrupt priority */
	NVIC_EnableIRQ(USART1_IRQn);						/* Enable IRQ in NVIC */
	
	SET_BIT(USART1->CR1, USART_CR1_IDLEIE);	/* IDLE interrupt enabled CR1.IDLEIE */
	SET_BIT(USART1->ICR, USART_ICR_IDLECF);	/* Clear flag IDLE */
	
	SET_BIT(USART1->CR3, USART_CR3_EIE);	/* Error interrupt enabled CR3.EIE */
	SET_BIT(USART1->ICR, USART_ICR_FECF);	/* Clear flag FE */
	SET_BIT(USART1->ICR, USART_ICR_ORECF);	/* Clear flag ORE */
	SET_BIT(USART1->ICR, USART_ICR_NECF);	/* Clear flag NE */
	SET_BIT(USART1->ICR, USART_ICR_UDRCF);	/* Clear flag UDR */
}

static inline void InitializeDMA(void)
{
	/* p.310 - clock enable */
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMAMUX1);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

	/* p.408 - configure DMA channel */

	/* Disable channel CCR.EN */
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);

	/* Set DMA request for DMA instance on Channel 1 CSELR.CxS */
	LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMAMUX_REQ_USART1_RX);
	
	/* Configure the Source and Destination addresses CPAR.PA, CMAR.MA */
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, (uint32_t)&USART1->RDR, (uint32_t)&_rxDmaBuf[0], LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

	/* Set Number of data to transfer CNDTR.NDT */
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, RX_BUFFER_SIZE);
	
	/* Set Data transfer direction CCR.DIR, CCR.MEM2MEM */
	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	
	LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_NORMAL);	/* CCR.CIRC */
	
	/* Increment address index */
	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_NOINCREMENT);	/* PINC */
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);	/* MINC */
	
	/* Data size */
	LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_BYTE);	/* CCR.PSIZE */
	LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_BYTE);	/* CCR.MSIZE */
	
	LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_LOW);	/* CCR.PL */
	
	/* Set IT */
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);	/* CCR.TCIE Transfert complete interrupt enabled */
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1); /* CCR.TEIE Transfer error interrupt enabled */
	
	/* Clear interrupt flag */
	SET_BIT(DMA1->IFCR, LL_DMA_IFCR_CTCIF1);
	SET_BIT(DMA1->IFCR, LL_DMA_IFCR_CTEIF1);

	/* Enable IRQ */
	NVIC_SetPriority(DMA1_Channel1_IRQn, DMA1_CH1_INT_PRIORITY);	/* Set interrupt priority */
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);								/* Enable IRQ in NVIC */
	
	LL_USART_EnableDMAReq_RX(USART1);	/* USART1 DMA enabled */

	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}


static inline void TransmitRemainingData(void)
{
	uint8_t data[USART_FIFO_SIZE+1];
	uint16_t numberOfBytesRead, i;

	numberOfBytesRead = STREAM_Read(&_txStream, data, USART_FIFO_SIZE);
	if (numberOfBytesRead > 0)
	{
		for (i=0; i<numberOfBytesRead; i++)
			USART1->TDR = data[i];
	}
	else
	{
		/* Nothing left to transfer - remove IT */
		LL_USART_DisableIT_TXFE(USART1);
	}
}

bool USART1_Write(const uint8_t * const pBuf, uint16_t length)
{	
	/* Is TX FIFO empty interrupt enabled ? */
	if ((USART1->CR1 & USART_CR1_TXFEIE) == USART_CR1_TXFEIE)
		LL_USART_DisableIT_TXFE(USART1);

	if (USART1->ISR & USART_ISR_TXE_TXFNF)
	{/* TXFIFO not full */

		if (length > 1)
		{
			(void)STREAM_Write(&_txStream, &pBuf[1], length-1);
		}

		USART1->TDR = pBuf[0];
	}
	else
	{
		(void)STREAM_Write(&_txStream, pBuf, length);
	}

	/* Enable FIFO empty interrupt --> auto write remaining data from interrupt */
	LL_USART_EnableIT_TXFE(USART1);

	return true;
}


void USART1_IRQHandler(void)
{
	/* Receive */
	if (USART1->ISR & USART_ISR_IDLE)
	{
		/* Clear flag IDLE */
		SET_BIT(USART1->ICR, USART_ICR_IDLECF);

		uint32_t numberOfBytesRead = RX_BUFFER_SIZE - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_1);
		(void)numberOfBytesRead;

		/* Reload DMA */
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, RX_BUFFER_SIZE);
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
	}

	/* Transmit */
	if (LL_USART_IsActiveFlag_TXFE(USART1))
	{/* TXFE: TXFIFO empty */
		
		TransmitRemainingData();
	}
	

	/* ERROR - CR3.EIE */
	
	if (USART1->ISR & USART_ISR_FE)
	{
		SET_BIT(USART1->ICR, USART_ICR_FECF);	/* Clear flag FE */
	}

	if (USART1->ISR & USART_ISR_ORE)
	{
		SET_BIT(USART1->ICR, USART_ICR_ORECF);	/* Clear flag ORE */
	}

	if (USART1->ISR & USART_ISR_NE)
	{
		SET_BIT(USART1->ICR, USART_ICR_NECF);	/* Clear flag NE */
	}

	if (USART1->ISR & USART_ISR_UDR)
	{
		SET_BIT(USART1->ICR, USART_ICR_UDRCF);	/* Clear flag UDR */
	}
}

void DMA1_Channel1_IRQHandler(void)
{
	/* Transfer complete */
	if (DMA1->ISR & LL_DMA_ISR_TCIF1)
	{
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, RX_BUFFER_SIZE);
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

		DMA1->IFCR |= LL_DMA_IFCR_CTCIF1;
	}

	/* Transfer error */
	if (DMA1->ISR & LL_DMA_ISR_TEIF1)
	{
		DMA1->IFCR |= LL_DMA_IFCR_CTEIF1;
	}
}
