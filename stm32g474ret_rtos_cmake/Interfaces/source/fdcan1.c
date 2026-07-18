/**
 * @file can.c
 * @brief CAN Interface
 * 
 * @author Phuc VU
 * @date Nov 13, 2025
 * @note Using stm32g_hal_fdcan.c as model
 */

/********************************* Includes ***************************************/

#include <stdint.h>

#include <stm32g4xx.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_gpio.h>

#include "fdcan1.h"
#include "gpio.h"

/********************************* Constants definition ***************************/

#define FDCAN1_INT_PRIORITY                      10U

#define CAN_NOMINAL_SYNC_JUMP_WIDTH               1U
#define CAN_NOMINAL_PRESCALER                     3U
#define CAN_NOMINAL_SEG1                         13U
#define CAN_NOMINAL_SEG2                          2U

#define CAN_DATA_SYNC_JUMP_WIDTH                   1U
#define CAN_DATA_PRESCALER                         3U
#define CAN_DATA_SEG1                             13U
#define CAN_DATA_SEG2                              2U
#define CAN_DATA_TRANSCEIVER_DELAY_COMPENSATION    0U

/********************************* Macros *****************************************/

static const uint8_t DLCtoBytes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};

/********************************* Types definition *******************************/

/********************************* Local variable *********************************/

static FDCAN_MsgRamAddressTypeDef  _msgRam;           /*!< FDCAN Message RAM blocks  */

static uint32_t                    _LatestTxFifoQRequest; /*!< FDCAN Tx buffer index
                                                    of latest Tx FIFO/Queue request */


static FDCAN_RxHeaderTypeDef canRxHeader;
static uint8_t canRxData[8];

/********************************* Private prototypes *****************************/


/********************************* Prototypes *************************************/

static inline void CAN1_InitGPIO(void);
static inline void CAN1_SelectMode(void);
static inline void CAN1_ConfigBaudrate(void);
static inline void CAN1_ConfigMemory(void);
static inline void CAN1_EnableInterrupt(void);

/********************************* Global variables *******************************/

/********************************* Implementations ********************************/

void FDCAN1_Initialize(void)
{   
    /*
    * clock HSE 24 MHz
    * CAN 1,2,3 -> ABP1
    * 
    * CAN extended non FD
    * http://www.bittiming.can-wiki.info/
    * 
    * 500 kbits
    * PA11-RX, PA12-TX
    * 
    * p.1966 standard id filter
    * p.1967 extended id filter
    */

    CAN1_InitGPIO();

    /* Set clock */
    LL_RCC_SetFDCANClockSource(LL_RCC_FDCAN_CLKSOURCE_HSE); /* Use external clock source */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_FDCAN);    /* Peripheral clock enable */

    FDCAN_WakeUp(FDCAN1);    
    FDCAN_EnterInitMode(FDCAN1);
    
    /* CCCR.CCE Configuration change enable */
    SET_BIT(FDCAN1->CCCR, FDCAN_CCCR_CCE); /* Enable configuration change */

    /* FDCAN CFG clock divider register (default DIV1 = 0b0000)*/
    // FDCAN_CONFIG->CKDIV 

    /* Set the no automatic retransmission (default enabled) */
    FDCAN_DisableAutoRetransmission(FDCAN1);

    /* Set the transmit pause feature (default disabled ) */
    //FDCAN_EnablePauseTransmit(FDCAN1);

    /* CCCR.EFBI Edge filtering during bus integration (default disabled) */
    //SET_BIT(FDCAN1->CCCR, FDCAN_CCCR_EFBI); /* Enabled */

    /* CCCR.BRSE FDCAN bit rate switching (default disabled)*/
    //SET_BIT(FDCAN1->CCCR, FDCAN_CCCR_BRSE); /* Enabled */

    /* CCCR.FDOE FD Operation Enable (default disabled) */
    //SET_BIT(FDCAN1->CCCR, FDCAN_CCCR_FDOE); /* Enabled */

    /* CCCR.PXHD Protocol Exception Handling enabled */
    FDCAN_DisableProtocolExceptionHandling(FDCAN1);

    CAN1_SelectMode();

    CAN1_ConfigBaudrate();

    CAN1_ConfigMemory();

    CAN1_EnableInterrupt();

    FDCAN_LeaveInitMode(FDCAN1);

    // flush
    while (FDCAN1_GetMessage(&canRxHeader, canRxData) == true);
}

static inline void CAN1_InitGPIO(void)
{
    /* p.17 DS12288 clock assign */
    /* Is clock enabled ? */
    if (READ_BIT(RCC->AHB2ENR, LL_AHB2_GRP1_PERIPH_GPIOA) != LL_AHB2_GRP1_PERIPH_GPIOA)
	{
		LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
	}

    /* ALTERNATE FUNCTION (AFx) find in doc DS12288 p.73 */

	/* PA11     ------> FDCAN1_RX */
	LL_GPIO_SetPinMode      (GPIOA, LL_GPIO_PIN_11, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_11, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed     (GPIOA, LL_GPIO_PIN_11, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinPull      (GPIOA, LL_GPIO_PIN_11, LL_GPIO_PULL_NO);
	LL_GPIO_SetAFPin_8_15   (GPIOA, LL_GPIO_PIN_11, LL_GPIO_AF_9);

	/* Finally lock pin configuration */
	//LL_GPIO_LockPin(GPIOA, LL_GPIO_PIN_11);

	// PA12     ------> FDCAN1_TX
	LL_GPIO_SetPinMode      (GPIOA, LL_GPIO_PIN_12, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_12, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed     (GPIOA, LL_GPIO_PIN_12, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinPull      (GPIOA, LL_GPIO_PIN_12, LL_GPIO_PULL_NO);
	LL_GPIO_SetAFPin_8_15   (GPIOA, LL_GPIO_PIN_12, LL_GPIO_AF_9);

	/* Finally lock pin configuration */
	//LL_GPIO_LockPin(GPIOA, LL_GPIO_PIN_12);
}

static inline void CAN1_EnableInterrupt(void)
{
    
    SET_BIT(FDCAN1->IE, FDCAN_IE_RF0NE); /* RF0NE: Rx FIFO 0 new message interrupt enable (default disabled) : Enabled */
    //SET_BIT(FDCAN1->IE, FDCAN_IE_RF1NE); /* RF0NE: Rx FIFO 1 new message interrupt enable (default disabled) : Enabled */

    /* Assign group interrupt enable */
    //SET_BIT(FDCAN1->ILS, FDCAN_ILS_RXFIFO0);

    SET_BIT(FDCAN1->ILE, FDCAN_ILE_EINT0); /* Enable interrupt line 0. */
    //SET_BIT(FDCAN1->ILE, FDCAN_ILE_EINT1); /* Enable interrupt line 1. */

    /* Set priority of Interrupt */
	NVIC_SetPriority(FDCAN1_IT0_IRQn, FDCAN1_INT_PRIORITY);
    //NVIC_SetPriority(FDCAN1_IT1_IRQn, FDCAN1_INT_PRIORITY);

	/* Enable IRQ in NVIC */
	NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
    //NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
}

static inline void CAN1_ConfigMemory(void)
{
    /* 
    * P.1961 RX FIFO element
    * p.1963 TX FIFO element
    */

    /* FDCAN_TXBC: Tx buffer configuration FIFO (default) or queue */
    //SET_BIT(FDCAN1->TXBC, FDCAN_TXBC_TFQM); /* Select Tx queue operation */


    uint32_t RAMcounter;

    /* Mode: blocking or override */
    CLEAR_BIT(FDCAN1->RXGFC, FDCAN_RXGFC_F0OM); /* FIFO 0 mode blocking */
    //CLEAR_BIT(FDCAN1->RXGFC, FDCAN_RXGFC_F1OM); /* FIFO 1 mode blocking */

    /* Standard filter list start address */
    _msgRam.StandardFilterSA = SRAMCAN_BASE + SRAMCAN_FLSSA;

    /* RXGFC.LSS Standard filter elements number (default 0) */
    //MODIFY_REG(FDCAN1->RXGFC, FDCAN_RXGFC_LSS, (n << FDCAN_RXGFC_LSS_Pos));

    /* Extended filter list start address */
    _msgRam.ExtendedFilterSA = SRAMCAN_BASE + SRAMCAN_FLESA;

    /* RXGFC.LSE Extended filter elements number (default 0) */
    //MODIFY_REG(FDCAN1->RXGFC, FDCAN_RXGFC_LSE, (ne << FDCAN_RXGFC_LSE_Pos));

    /* Rx FIFO 0 start address */
    _msgRam.RxFIFO0SA = SRAMCAN_BASE + SRAMCAN_RF0SA;

    /* Rx FIFO 1 start address */
    _msgRam.RxFIFO1SA = SRAMCAN_BASE + SRAMCAN_RF1SA;

    /* Tx event FIFO start address */
    _msgRam.TxEventFIFOSA = SRAMCAN_BASE + SRAMCAN_TEFSA;

    /* Tx FIFO/queue start address */
    _msgRam.TxFIFOQSA = SRAMCAN_BASE + SRAMCAN_TFQSA;

    /* Flush the allocated Message RAM area */
    for (RAMcounter = SRAMCAN_BASE; RAMcounter < (SRAMCAN_BASE + SRAMCAN_SIZE); RAMcounter += 4U)
    {
        *(uint32_t *)(RAMcounter) = 0x00000000U;
    }

    _LatestTxFifoQRequest = 0U;
}

static inline void CAN1_ConfigBaudrate(void)
{
    FDCAN1->NBTP = (((CAN_NOMINAL_SYNC_JUMP_WIDTH - 1) << FDCAN_NBTP_NSJW_Pos) & FDCAN_NBTP_NSJW_Msk) | \
                   (((CAN_NOMINAL_PRESCALER - 1) << FDCAN_NBTP_NBRP_Pos) & FDCAN_NBTP_NBRP_Msk) | \
                   (((CAN_NOMINAL_SEG1 - 1) << FDCAN_NBTP_NTSEG1_Pos) & FDCAN_NBTP_NTSEG1_Msk) | \
                   ((CAN_NOMINAL_SEG2 - 1) & FDCAN_NBTP_NTSEG2_Msk);

    #if 0
    if (READ_BIT(FDCAN1->CCCR, FDCAN_CCCR_BRSE | FDCAN_CCCR_FDOE) == (FDCAN_CCCR_BRSE | FDCAN_CCCR_FDOE))
    {
        FDCAN1->DBTP = (((CAN_DATA_PRESCALER - 1 ) << FDCAN_DBTP_DBRP_Pos) & FDCAN_DBTP_DBRP_Msk) | \
                       (((CAN_DATA_SEG1 - 1 ) << FDCAN_DBTP_DTSEG1_Pos) & FDCAN_DBTP_DTSEG1_Msk) | \
                       (((CAN_DATA_SEG2 - 1 ) << FDCAN_DBTP_DTSEG2_Pos) & FDCAN_DBTP_DTSEG2_Msk) | \
                       ((CAN_DATA_SYNC_JUMP_WIDTH - 1) & FDCAN_DBTP_DSJW_Msk);
    
        /* Transceiver delay compensation (default disabled) */
        //SET_BIT(FDCAN1->DBTP, FDCAN_DBTP_TDC);
    }
    #endif
}

static inline void CAN1_SelectMode(void)
{
    /* Set FDCAN Operating Mode:
                 | Normal | Restricted |    Bus     | Internal | External
                 |        | Operation  | Monitoring | LoopBack | LoopBack
       CCCR.TEST |   0    |     0      |     0      |    1     |    1
       CCCR.MON  |   0    |     0      |     1      |    1     |    0
       TEST.LBCK |   0    |     0      |     0      |    1     |    1
       CCCR.ASM  |   0    |     1      |     0      |    0     |    0
    */

    /* Reset FDCAN Operation Mode -> mode normal */
    CLEAR_BIT(FDCAN1->CCCR, FDCAN_CCCR_TEST | FDCAN_CCCR_MON | FDCAN_CCCR_ASM);
    CLEAR_BIT(FDCAN1->TEST, FDCAN_TEST_LBCK);
}


bool FDCAN1_GetMessage(FDCAN_RxHeaderTypeDef * pRxHeader, uint8_t * pRxData)
{
    /*
    * P.1961 RX FIFO element
    */

    static int fullCount = 0;

    uint32_t *RxAddress;
    uint8_t  *pData;
    uint32_t ByteCounter;
    uint32_t GetIndex = 0;
    uint32_t fillLevel;

    /* F0FL[3:0] -> range 0 to 3 */
    fillLevel = FDCAN1->RXF0S & FDCAN_RXF0S_F0FL;
    if (fillLevel > 0)
    {
        /* FDCAN1->RXF0S -> Rx FIFO 0 status */
        /* FIFO 0 full ? */
        if (((FDCAN1->RXF0S & FDCAN_RXF0S_F0F) >> FDCAN_RXF0S_F0F_Pos) == 1U)
        {
            fullCount++;
            (void)fullCount;
        }
        
        /* Calculate Rx FIFO 0 element index */
        GetIndex += ((FDCAN1->RXF0S & FDCAN_RXF0S_F0GI) >> FDCAN_RXF0S_F0GI_Pos);
        
        /* Calculate Rx FIFO 0 element address */
        RxAddress = (uint32_t *)(_msgRam.RxFIFO0SA + (GetIndex * SRAMCAN_RF0_SIZE));
        
       
        /* Retrieve IdType */
        pRxHeader->IdType = *RxAddress & FDCAN_ELEMENT_MASK_XTD;

        /* Retrieve Identifier */
        if (pRxHeader->IdType == FDCAN_STANDARD_ID) /* Standard ID element */
        {
            pRxHeader->Identifier = ((*RxAddress & FDCAN_ELEMENT_MASK_STDID) >> 18U);
        }
        else /* Extended ID element */
        {
            pRxHeader->Identifier = (*RxAddress & FDCAN_ELEMENT_MASK_EXTID);
        }

        /* Retrieve RxFrameType */
        pRxHeader->RxFrameType = (*RxAddress & FDCAN_ELEMENT_MASK_RTR);

        /* Retrieve ErrorStateIndicator */
        pRxHeader->ErrorStateIndicator = (*RxAddress & FDCAN_ELEMENT_MASK_ESI);

        /* Increment RxAddress pointer to second word of Rx FIFO element */
        RxAddress++;

        /* Retrieve RxTimestamp */
        pRxHeader->RxTimestamp = (*RxAddress & FDCAN_ELEMENT_MASK_TS);

        /* Retrieve DataLength */
        pRxHeader->DataLength = ((*RxAddress & FDCAN_ELEMENT_MASK_DLC) >> 16U);

        /* Retrieve BitRateSwitch */
        pRxHeader->BitRateSwitch = (*RxAddress & FDCAN_ELEMENT_MASK_BRS);

        /* Retrieve FDFormat */
        pRxHeader->FDFormat = (*RxAddress & FDCAN_ELEMENT_MASK_FDF);

        /* Retrieve FilterIndex */
        pRxHeader->FilterIndex = ((*RxAddress & FDCAN_ELEMENT_MASK_FIDX) >> 24U);

        /* Retrieve NonMatchingFrame */
        pRxHeader->IsFilterMatchingFrame = ((*RxAddress & FDCAN_ELEMENT_MASK_ANMF) >> 31U);

        /* Increment RxAddress pointer to payload of Rx FIFO element */
        RxAddress++;

        /* Retrieve Rx payload */
        pData = (uint8_t *)RxAddress;
        for (ByteCounter = 0; ByteCounter < DLCtoBytes[pRxHeader->DataLength]; ByteCounter++)
        {
            pRxData[ByteCounter] = pData[ByteCounter];
        }
        
        /* Acknowledge the Rx FIFO 0 that the oldest element is read so that it increments the GetIndex */
        FDCAN1->RXF0A = GetIndex;
        
        return true;
    }

    return false;
}

__attribute__((weak)) void FDCAN1_RX_Callback( uintptr_t context )
{
    static const char * msg = "FDCAN1 RX";
    int i;
    uint32_t fillLevel = *(uint32_t*)context;

    if (fillLevel > 0)
    {
        if (FDCAN1_GetMessage(&canRxHeader, canRxData))
        {
            if (FDCAN_REMOTE_FRAME != canRxHeader.RxFrameType)
            {
                if (FDCAN_STANDARD_ID != canRxHeader.IdType)
                {
                	i = 0;
                    do
                    {
                        ITM_SendChar(msg[i]);
                    }while(msg[++i] != '\0');

                    ITM_SendChar('\n');
                }
            }
        }
    }
}

void FDCAN1_IT0_IRQHandler(void)
{   
    uint32_t fillLevel;

    uint32_t rxFifo0Int = FDCAN_RX_FIFO0_MASK & FDCAN1->IR;
    rxFifo0Int &= FDCAN1->IE;

    //uint32_t rxFifo1Int = FDCAN_RX_FIFO1_MASK & FDCAN1->IR;
    //rxFifo1Int &= FDCAN1->IE;

    /* Rx FIFO 0 interrupt */
    if (rxFifo0Int != 0)
    {
        __HAL_FDCAN_CLEAR_FLAG(FDCAN1, rxFifo0Int);

        /* Rx FIFO 0 new message */
        if (rxFifo0Int & FDCAN_IR_RF0N)
        {
            fillLevel = FDCAN1->RXF0S & FDCAN_RXF0S_F0FL;
            FDCAN1_RX_Callback((uintptr_t)&fillLevel);
        }
    
        /* Rx FIFO 0 Message lost */
        if ((rxFifo0Int & FDCAN_IR_RF0L) == FDCAN_IR_RF0L)
        {
        }
    
        /* Rx FIFO 0 full */
        if ((rxFifo0Int & FDCAN_IR_RF0F) == FDCAN_IR_RF0F)
        {
        }
    }

    /* Rx FIFO 1 interrupt 
    if (rxFifo1Int != 0)
    {
        __HAL_FDCAN_CLEAR_FLAG(FDCAN1, rxFifo1Int);
    }
    */
}

void FDCAN1_IT1_IRQHandler(void)
{
    
}

/*EOF*/
