/**
 * @file lpuart1.h
 * @brief Low Power UART1 interface
 * @details This file contains the interface for Low Power UART1 operations.
 * 
 * @author Phuc VU
 * @date Dec 7, 2024
 */

/********************************* Includes ***************************************/
#include <stdio.h>
#include <stm32g4xx.h>
#include <stm32g4xx_ll_usart.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_bus.h>

#include "lpuart1.h"
#include "stream.h"


/********************************* Macros *****************************************/
#define FIFO_ENABLED	    1
#define INTERRUPT_ENABLED	1

#define LPUART1_INT_PRIORITY	    7U

#define RX_BUFFER_SIZE 128U
#define TX_BUFFER_SIZE 128U

/********************************* Local variables ********************************/
static uint8_t _rxBuf[RX_BUFFER_SIZE];
static uint8_t _txBuf[TX_BUFFER_SIZE];

static Stream_t _rxStream;
static Stream_t _txStream;

/********************************* Local prototypes *******************************/
static inline void InitializeGPIO(void);
static inline void InitializeLPUART(void);

static inline void TransmitRemainingData(void);

void LPUART1_IRQHandler(void);

/********************************* Implementations ********************************/

void LPUART1_Initialize(void)
{
	InitializeGPIO();
	InitializeLPUART();

	STREAM_Initialize(&_rxStream, _rxBuf, RX_BUFFER_SIZE);
	STREAM_Initialize(&_txStream, _txBuf, TX_BUFFER_SIZE);
}

static inline void InitializeGPIO(void)
{
	/* !!! NOTE : ALTERNATE FUNCTION (AFx) find in doc DS12288 p.73 */

	/* Pin PA2/PA3 */

	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

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


static inline void InitializeLPUART(void)
{
	// p.1684 Character transmission procedure ---> configuration

	/* UART parameters:
	 * bauds 115200
	 * 8 bits
	 * 1 stop
	 * parity none
	 */

	/* Disable LPUART CR1.UE */
	LL_USART_Disable(LPUART1);

	/* Clock LPUART (RCC) */
	LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_PCLK1);
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_LPUART1);

	//LL_USART_ConfigCharacter(LPUART1, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
	//LL_USART_SetBaudRate(LPUART1, 25000000, LL_USART_PRESCALER_DIV1, LL_USART_OVERSAMPLING_16, 115200);


	// Async mode : default
	//LL_USART_DisableHalfDuplex(LPUART1);

	// Parity CR1.PCE & CR1.PS : default NONE
	//

	// Data length CR1.M1 & CR1.M0 : default 1 Start bit, 8 Data bits, n Stop bit
	//

	// Stop bits : default 1
	//CLEAR_BIT(LPUART1->CR2, USART_CR2_STOP);

	// Prescaler default = DIV1
	//LL_USART_SetPrescaler(LPUART1, LL_USART_PRESCALER_DIV1);

	// Baud rate 115200
	LPUART1->BRR = 0xd904;

	// Receive enable CR1.RE / Transmitter enable CR1.TE
	LL_USART_SetTransferDirection(LPUART1, LL_USART_DIRECTION_TX_RX);


#if (FIFO_ENABLED == 1)
	// CR1.FIFOEN Enable FIFO
	SET_BIT(LPUART1->CR1, USART_CR1_FIFOEN);

	// Set FIFO threshold - default LL_USART_FIFOTHRESHOLD_1_8
	//LL_USART_SetTXFIFOThreshold(LPUART1, LL_USART_FIFOTHRESHOLD_1_8);
	//LL_USART_SetRXFIFOThreshold(LPUART1, LL_USART_FIFOTHRESHOLD_1_4);
#endif


#if (INTERRUPT_ENABLED == 1)

	// Enable Transmitter complete interrupt
	//LL_USART_EnableIT_TC(LPUART1); // trig all the time --> error?

	// Enable FIFO empty interrupt
	//SET_BIT(LPUART1->CR1, USART_CR1_TXFEIE);
	//LL_USART_EnableIT_TXFE(LPUART1);

	// Transmit data register empty interrupt
	//SET_BIT(LPUART1->CR1, USART_CR1_TXEIE);

	// CR3 Bit 23 TXFTIE: TXFIFO threshold interrupt enable
	//SET_BIT(LPUART1->CR3, USART_CR3_TXFTIE);

	// CR3 Bit 28 RXFTIE: RXFIFO threshold interrupt enable --> USART_ISR_RXFT/USART_ISR_RXNE_RXFNE
	//SET_BIT(LPUART1->CR3, USART_CR3_RXFTIE);

	/* Enable Rx Interrupt : RX Not Empty --> USART_ISR_RXFT/USART_ISR_RXNE_RXFNE */
	LL_USART_EnableIT_RXNE_RXFNE(LPUART1);

	// CR3 Bit 0 EIE: Error interrupt enable --> USART_ISR_ORE/USART_ISR_NE/USART_ISR_FE
	SET_BIT(LPUART1->CR3, USART_CR3_EIE);

	/* Set priority of Interrupt */
	NVIC_SetPriority(LPUART1_IRQn, LPUART1_INT_PRIORITY);

	/* Enable IRQ in NVIC */
	NVIC_EnableIRQ(LPUART1_IRQn);

#endif

	// Enable LPUART CR1.UE
	LL_USART_Enable(LPUART1);
}

void LPUART1_WriteOneData(const uint8_t value)
{
#if (FIFO_ENABLED == 1)

	// Transmitter Enable
	//LL_USART_EnableDirectionTx(LPUART1);

	// TC = 1 --> Transmission complete
	while(READ_BIT(LPUART1->ISR, USART_ISR_TC) != USART_ISR_TC);

	// LPUART_ISR.TXFNF = 1 --> TXFIFO not full
	if (LL_USART_IsActiveFlag_TXE_TXFNF(LPUART1))
	{
		// add data to transmit
		LPUART1->TDR = value;
	}

	// Transmitter Disable
	//LL_USART_DisableDirectionTx(LPUART1);

#else
	/* FIFO disabled */
	// TXE transmit data register empty
	if (READ_BIT(LPUART1->ISR, USART_ISR_TXE) == USART_ISR_TXE)
	{
		// add data to transmit
		LPUART1->TDR = value;
	}
#endif
}

/*** TRANSMIT FUNCTIONS ************************************************/

bool LPUART1_Write( uint8_t const * const pBuf, uint16_t length)
{
	uint8_t data = 0;
	uint16_t nLu = 0;

	if ((!pBuf) || (!length))
	{
		return false;
	}
	
	
	NVIC_DisableIRQ(LPUART1_IRQn);
	
	/* Copy to stream */
	(void)STREAM_Write(&_txStream, pBuf, length);
	
	/* Fill TXFIFO buffer */
	while(LPUART1->ISR & USART_ISR_TXE_TXFNF)
	{/* TXFIFO not full */
		nLu = STREAM_Read(&_txStream, &data, 1);
		
		if (nLu)
		{
			LPUART1->TDR = data;
		}
		else
		{
			break;
		}
	}	

	NVIC_EnableIRQ(LPUART1_IRQn);
	

	/* If TX interrupt is enabled, do not proceed here */
	if (!(LPUART1->CR1 & USART_CR1_TXFEIE))
	{
		// Enable FIFO empty interrupt --> auto write remaining data from interrupt
		LL_USART_EnableIT_TXFE(LPUART1);
	}

	return true;
}

static inline void TransmitRemainingData(void)
{
	uint8_t data = 0;
	uint16_t nLu = 0;

	nLu = STREAM_Read(&_txStream, &data, 1);

	if (nLu)
	{
		LPUART1->TDR = data;

		/* Continue to fill the TXFIFO */
		do
		{
			/* is TXFIFO full ? */
			if ( !(LPUART1->ISR & USART_ISR_TXE_TXFNF) )
				break;

			nLu = STREAM_Read(&_txStream, &data, 1);

			if ( !nLu )
				break;

			LPUART1->TDR = data;

		}while(nLu);
	}
	else
	{
		// remove IT if nothing left from stream
		LL_USART_DisableIT_TXFE(LPUART1);
	}
}

/*** RECEIVE FUNCTIONS ************************************************/

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
	// CR1.RXNEIE = 1 --> RXNE/RXFNE : RXFIFO not empty
	if (LPUART1->ISR & USART_ISR_RXNE_RXFNE)
	{
		//uint8_t c = LPUART1->RDR;
		(void)STREAM_Write(&_rxStream, (uint8_t*)&LPUART1->RDR, 1);
	}

	// CR3.RXFTIE = 1 & CR3.RXFTCFG --> RX threshold trigger
	//if (LPUART1->ISR & USART_ISR_RXFT) {}

	// CR3.TXFTIE = 1 --> TX threshold trigger
	//if (LPUART1->ISR & USART_ISR_TXFT) {}

	// Transmit data register empty/TXFIFO not full
	//if (LPUART1->ISR & USART_ISR_TXE) {}

	// Bit 23 TXFE: TXFIFO empty
	if (LPUART1->ISR & USART_ISR_TXFE)
	{
		TransmitRemainingData();
	}

	// Transmit complete --> use with FIFO disabled
	//if (LPUART1->ISR & USART_ISR_TC) { TransmitDisable(); }


	// ERROR FLAG --> CR3.EIE = 1
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

void LPUART1_TestTransmit(void)
{
	uint8_t msg[] = "LPUART1 Test Transmit\r\n";
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
