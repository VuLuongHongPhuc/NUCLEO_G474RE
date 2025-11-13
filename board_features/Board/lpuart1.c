/*
 * lpuart1.c
 *
 *  Created on: Dec 7, 2024
 *      Author: admin
 */


#include "../Board/lpuart1.h"

#include "../LL/stm32g4xx.h"
#include "../LL/stm32g4xx_ll_bus.h"
#include "../LL/stm32g4xx_ll_gpio.h"
#include "../LL/stm32g4xx_ll_rcc.h"
#include "../LL/stm32g4xx_ll_usart.h"




/*** LOCAL DEFINE *****************************************************/
#define FIFO_ENABLED	    1
#define INTERRUPT_ENABLED	1

#define RX_BUFFER_SIZE 128
#define TX_BUFFER_SIZE 128

/*** LOCAL VARIABLES **************************************************/
static uint8_t RxBuffer[RX_BUFFER_SIZE];
static Stream_t RxStream;

static uint8_t TxBuffer[TX_BUFFER_SIZE];
static Stream_t TxStream;

/*** LOCAL PROTOTYPE **************************************************/
static inline void InitializeGPIO(void);
static inline void InitializeLPUART(void);
static inline void WriteFromISR(void);
static inline void FillRxStreamFromISR(void);

void LPUART1_IRQHandler(void);


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
	NVIC_SetPriority(LPUART1_IRQn, 7);

	/* Enable IRQ in NVIC */
	NVIC_EnableIRQ(LPUART1_IRQn);

#endif

	// Enable LPUART CR1.UE
	LL_USART_Enable(LPUART1);
}


/*** API **************************************************************/

/**
 * @brief LPUART initialization
 * @retval None
 * */
void LPUART1_Initialize(void)
{
	InitializeGPIO();
	InitializeLPUART();

	STREAM_Init(&RxStream, RxBuffer, RX_BUFFER_SIZE-1);
	STREAM_Init(&TxStream, TxBuffer, TX_BUFFER_SIZE-1);
}



/*** TRANSMIT FUNCTIONS ***********************************************/

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



void LPUART1_Write(const uint8_t* buff, uint8_t length)
{
	uint8_t data = 0;
	uint8_t read = 0;

	if (length == 0)
	{
		return;
	}

	//---- Copy to stream
	STREAM_Write(&TxStream, buff, length);


	//---- Fill TXFIFO buffer
	// while TXFIFO not full
	while(LPUART1->ISR & USART_ISR_TXE_TXFNF)
	{
		STREAM_Read(&TxStream, &data, 1, &read);

		if (read)
		{
			LPUART1->TDR = data;
		}
		else
		{
			break;
		}
	}

	// Enable FIFO empty interrupt --> auto write from interrupt the remaining
	LL_USART_EnableIT_TXFE(LPUART1);
}

static inline void WriteFromISR(void)
{
	uint8_t data = 0;
	uint8_t read = 0;

	STREAM_Read(&TxStream, &data, 1, &read);

	if (read)
	{
		LPUART1->TDR = data;

#define CONTINU_FILL_TX_FIFO	1
#if (CONTINU_FILL_TX_FIFO == 1)

		// Continue to fill the TXFIFO
		do
		{
			// if TXFIFO full
			if ( !(LPUART1->ISR & USART_ISR_TXE_TXFNF) )
				break;

			STREAM_Read(&TxStream, &data, 1, &read);

			if ( !read )
				break;

			LPUART1->TDR = data;

		}while(read);
#endif

	}
	else
	{
		// remove IT if nothing left from stream
		LL_USART_DisableIT_TXFE(LPUART1);
	}
}

/*** RECEIVE FUNCTIONS ************************************************/

static inline void FillRxStreamFromISR(void)
{
	uint32_t c = LPUART1->RDR;
	STREAM_Write(&RxStream, (uint8_t*)&c, 1);
}

void LPUART1_Read(uint8_t* buffer, uint8_t length, uint8_t* read)
{
	STREAM_Read(&RxStream, buffer, length, read);
}

Stream_t* LPUART1_GetRxStream(void)
{
	return &RxStream;
}

/*** IRQ HANDLER ******************************************************/

// p.1704 RM0440 LPUART interrupts
void LPUART1_IRQHandler(void)
{

	// CR1.RXNEIE = 1 --> RXNE/RXFNE : RXFIFO not empty
	if (LPUART1->ISR & USART_ISR_RXNE_RXFNE)
	{
		FillRxStreamFromISR();
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
		WriteFromISR();
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



/*EOF*/
