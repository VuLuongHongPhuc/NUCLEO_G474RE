/**
 * @file lpuart1.c
 * @brief Low Power UART1 interface
 * @details This file contains the interface for Low Power UART1 operations.
 * 
 * @author Phuc VU
 * @date Dec 7, 2024
 */

/********************************* Includes ***************************************/
#include <stdio.h>
#include <stm32g4xx.h>
#include <stm32g4xx_ll_lpuart.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_dma.h>

#include "lpuart1.h"
#include "stream.h"


/********************************* Macros *****************************************/

#define INTERRUPT_ENABLED	1

#define FIFO_SIZE                8U

#define LPUART1_INT_PRIORITY	 7U

#define RX_BUFFER_DMA_SIZE       32U
#define RX_BUFFER_SIZE           128U
#define TX_BUFFER_SIZE           128U

/********************************* Local variables ********************************/

//static uint8_t _rxBufDma[RX_BUFFER_DMA_SIZE];
static uint8_t _rxBuf[RX_BUFFER_SIZE];
static uint8_t _txBuf[TX_BUFFER_SIZE];

static Stream_t _rxStream;
static Stream_t _txStream;

/********************************* Local prototypes *******************************/
static inline void InitializeGPIO(void);
static inline void InitializeLPUART(uint32_t pclk1_frequency);
static inline void InitializeInterrupt(void);

static inline void TransmitRemainingData(void);

void LPUART1_IRQHandler(void);

/********************************* Implementations ********************************/

/**
 * @brief LPUART initialization
 * @param[in] pclk1_frequency - PCLK1 frequency
 * @retval None
 */
void LPUART1_Initialize(uint32_t pclk1_frequency)
{
	InitializeGPIO();
	InitializeLPUART(pclk1_frequency);

	STREAM_Initialize(&_rxStream, _rxBuf, RX_BUFFER_SIZE);
	STREAM_Initialize(&_txStream, _txBuf, TX_BUFFER_SIZE);
}

static inline void InitializeGPIO(void)
{
	/* !!! NOTE : ALTERNATE FUNCTION (AFx) find in doc DS12288 p.73 */

	/* Pin PA2/PA3 */
	volatile uint32_t tmpreg = READ_BIT(RCC->AHB2ENR, LL_AHB2_GRP1_PERIPH_GPIOA);
    if (tmpreg != LL_AHB2_GRP1_PERIPH_GPIOA)
	{
		LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
	}

	/* PA2     ------> LPUART1_TX */
	LL_GPIO_SetPinMode      (GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed     (GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinPull      (GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_NO);
	LL_GPIO_SetAFPin_0_7    (GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_12);

	/* Finally lock pin configuration */
	//LL_GPIO_LockPin(GPIOA, LL_GPIO_PIN_2);

	// PA3     ------> LPUART1_RX
	LL_GPIO_SetPinMode      (GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_3, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed     (GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinPull      (GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_NO);
	LL_GPIO_SetAFPin_0_7    (GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_12);

	/* Finally lock pin configuration */
	//LL_GPIO_LockPin(GPIOA, LL_GPIO_PIN_3);
}


static inline void InitializeLPUART(uint32_t pclk1_frequency)
{
	// p.1684 Character transmission procedure ---> configuration

	/* UART parameters:
	 * LPUART1 @ 0x4000_8000
	 * bauds 115200
	 * 8 bits
	 * 1 stop
	 * parity none
	 * FIFO ON
	 */

	/* Disable LPUART CR1.UE */
	LL_LPUART_Disable(LPUART1);

	/* Clock LPUART (RCC) */
	LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_PCLK1);
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_LPUART1);

	// Async mode : default
	//LL_USART_DisableHalfDuplex(LPUART1);

	// Parity CR1.PCE & CR1.PS : default NONE

	// Data length CR1.M1 & CR1.M0 : default 1 Start bit, 8 Data bits, n Stop bit

	// Stop bits CR2.USART_CR2_STOP : default 1

	// Prescaler default = DIV1
	LL_LPUART_SetPrescaler(LPUART1, LL_LPUART_PRESCALER_DIV1);

	LL_LPUART_SetBaudRate(LPUART1, pclk1_frequency, LL_LPUART_PRESCALER_DIV1, 115200);
	//LPUART1->BRR = 0xd904;

	// Receive enable CR1.RE / Transmitter enable CR1.TE
	LL_LPUART_SetTransferDirection(LPUART1, LL_LPUART_DIRECTION_TX_RX);

	// CR1.FIFOEN
	LL_LPUART_EnableFIFO(LPUART1);

	// CR3.TXFTCFG - default LL_USART_FIFOTHRESHOLD_1_8
	//LL_LPUART_SetTXFIFOThreshold(LPUART1, LL_USART_FIFOTHRESHOLD_1_8); 


	InitializeInterrupt();


	// Enable LPUART CR1.UE
	LL_LPUART_Enable(LPUART1);
}

static inline void InitializeInterrupt(void)
{
	// Enable Transmitter complete interrupt
	//LL_USART_EnableIT_TC(LPUART1); // trig all the time

	// Enable TXFIFO empty interrupt (CR1, USART_CR1_TXFEIE)
	//LL_USART_EnableIT_TXFE(LPUART1);

	// Transmit data register empty interrupt
	//SET_BIT(LPUART1->CR1, USART_CR1_TXEIE);

	// CR3 Bit 23 TXFTIE: TXFIFO threshold interrupt enable
	//SET_BIT(LPUART1->CR3, USART_CR3_TXFTIE);

	// CR3 Bit 28 RXFTIE: RXFIFO threshold interrupt enable --> USART_ISR_RXFT/USART_ISR_RXNE_RXFNE
	//SET_BIT(LPUART1->CR3, USART_CR3_RXFTIE);

	/* Enable Rx Interrupt : RX Not Empty --> USART_ISR_RXFT/USART_ISR_RXNE_RXFNE */
	LL_LPUART_EnableIT_RXNE_RXFNE(LPUART1);

	/* CR3 Bit 0 EIE: Error interrupt enable --> USART_ISR_ORE/USART_ISR_NE/USART_ISR_FE */
	SET_BIT(LPUART1->CR3, USART_CR3_EIE);

	/* Set priority of Interrupt */
	NVIC_SetPriority(LPUART1_IRQn, LPUART1_INT_PRIORITY);

	/* Enable IRQ in NVIC */
	NVIC_EnableIRQ(LPUART1_IRQn);
}

/*** TRANSMIT FUNCTIONS ************************************************/

/**
 * @brief Write multiple data to LPUART1
 * @param pBuf Pointer to transmit buffer
 * @param length Number of data to write
 * @retval true if success, false otherwise
 */
bool LPUART1_Write( uint8_t const * const pBuf, uint16_t length)
{
	if ((!pBuf) || (!length))
	{
		return false;
	}
	
	/* Check if the LPUART TX FIFO Empty Interrupt is enabled */
	if (LPUART1->CR1 & USART_CR1_TXFEIE)
		LL_LPUART_DisableIT_TXFE(LPUART1);

	//NVIC_DisableIRQ(LPUART1_IRQn);
	
	if (LPUART1->ISR & USART_ISR_TXE_TXFNF)
	{/* TXFIFO not full */
		if (length > 1)
			(void)STREAM_Write(&_txStream, &pBuf[1], length-1);

		LPUART1->TDR = pBuf[0];
	}
	else
	{
		/* Copy buffer to stream */
		(void)STREAM_Write(&_txStream, &pBuf[0], length);
	}
	
	//NVIC_EnableIRQ(LPUART1_IRQn);
	
	LL_LPUART_EnableIT_TXFE(LPUART1);

	return true;
}

static inline void TransmitRemainingData(void)
{
	uint8_t data[FIFO_SIZE+1] = {0};
	uint16_t nLu = 0;
	uint16_t i;

	nLu = STREAM_Read(&_txStream, data, FIFO_SIZE);

	if (nLu)
	{
		for (i=0; i<nLu; i++)
		{
			LPUART1->TDR = data[i];
		}
	}
	else
	{
		/* Nothing left from stream -> remove IT  */
		LL_LPUART_DisableIT_TXFE(LPUART1);
	}
}

/*** RECEIVE FUNCTIONS ************************************************/

/**
 * @brief Read data from LPUART1
 * @param pBuf Pointer to receive buffer
 * @param length Number of data to read
 * @retval Number of data actually read
 */
uint16_t LPUART1_Read(uint8_t * pBuf, uint8_t length)
{
	return STREAM_Read(&_rxStream, pBuf, length);
}

Stream_t * LPUART1_GetRxStream(void)
{
	return &_rxStream;
}

/*** IRQ HANDLER ******************************************************/

// p.1704 RM0440 LPUART interrupts
void LPUART1_IRQHandler(void)
{
	/* Receive */
	if (LPUART1->ISR & USART_ISR_RXNE_RXFNE)
	{// CR1.RXNEIE = 1 --> RXNE/RXFNE : RXFIFO not empty
		//uint8_t c = LPUART1->RDR;
		(void)STREAM_Write(&_rxStream, (uint8_t*)&LPUART1->RDR, 1);
	}

	// CR3.RXFTIE = 1 & CR3.RXFTCFG --> RX threshold trigger
	//if (LPUART1->ISR & USART_ISR_RXFT) {}

	// CR3.TXFTIE = 1 --> TX threshold trigger
	//if (LPUART1->ISR & USART_ISR_TXFT) {}

	// Transmit data register empty/TXFIFO not full
	//if (LPUART1->ISR & USART_ISR_TXE) {}

	/* Transmit */
	if (LPUART1->ISR & USART_ISR_TXFE)
	{/* TXFE: TXFIFO empty */
		TransmitRemainingData();
	}

	/* ERROR FLAG --> CR3.EIE = 1 */

	if (LPUART1->ISR & USART_ISR_ORE)
	{
		SET_BIT(LPUART1->ICR, USART_ICR_ORECF);
	}

	if (LPUART1->ISR & USART_ISR_NE)
	{
		SET_BIT(LPUART1->ICR, USART_ICR_NECF);
	}

	if (LPUART1->ISR & USART_ISR_FE)
	{
		SET_BIT(LPUART1->ICR, USART_ICR_FECF);
	}
}

#if 0
void DMA1_Channel2_IRQHandler()
{
	/* Transfer complete */
	if (DMA1->ISR & LL_DMA_ISR_TCIF1)
	{
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, RX_BUFFER_SIZE);
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);

		DMA1->IFCR |= LL_DMA_IFCR_CTCIF2;
	}

	/* Transfer error */
	if (DMA1->ISR & LL_DMA_ISR_TEIF2)
	{
		DMA1->IFCR |= LL_DMA_IFCR_CTEIF2;
	}
}
#endif

void LPUART1_TestTransmit(void)
{
	//uint8_t msg[] = "LPUART1 Test Transmit\r\n";
	uint8_t msg[] = { 'V', 'R', 'O', 'O', 'M' };
	LPUART1_Write( msg, sizeof(msg)/sizeof(msg[0]));
}

void LPUART1_TestReceive(void)
{
	uint8_t msg[] = {0};
	uint16_t nLu = LPUART1_Read(msg, 1);
	if (nLu)
	{
		printf("LPUART1 receive: 0x%x\r\n", msg[0]);
	}
}

/*EOF*/
