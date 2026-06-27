/**
 * @file can.c
 * @brief CAN Interface
 * 
 * @author Phuc VU
 * @date Nov 13, 2025
 */

/********************************* Includes ***************************************/
#include <stm32g4xx.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_bus.h>
#include "can.h"

/********************************* Macros *****************************************/

//// FDCAN configuration: Frame format
/** @brief FDCAN Frame Format: Classic mode */
#define FDCAN_FRAME_CLASSIC   ((uint32_t)0x00000000U)
/** @brief FDCAN Frame Format: FD mode without BitRate Switching */
#define FDCAN_FRAME_FD_NO_BRS ((uint32_t)FDCAN_CCCR_FDOE)
/** @brief FDCAN Frame Format: FD mode with BitRate Switching */
#define FDCAN_FRAME_FD_BRS    ((uint32_t)(FDCAN_CCCR_FDOE | FDCAN_CCCR_BRSE))

/**
 * @enum CAN_operating_modes_e
 * @brief Selectable operating mode of the CAN peripheral.
 */
typedef enum
{
    FDCAN_MODE_NORMAL = 0,           /* Normal Operation mode          */
    FDCAN_MODE_RESTRICTED_OPERATION, /* Restricted Operation mode      */
    FDCAN_MODE_BUS_MONITORING,       /* Bus Monitoring mode            */
    FDCAN_MODE_INTERNAL_LOOPBACK,    /* Internal LoopBack mode         */
    FDCAN_MODE_EXTERNAL_LOOPBACK,    /* External LoopBack mode         */
} CAN_operating_modes_e;

/********************************* Private prototypes *****************************/

//static void CanConfigPins(void);
//static void CanConfigFilters(void);
static void CanEnterInitMode(void);
static void CanLeaveInitMode(void);
static void CanInitClock(void);
static void CanWakeUp(void);
static void CanSleep(void);
static void CanOperatingModeSet(uint8_t mode);

/********************************* Prototypes *************************************/
//void FDCAN1_IT0_IRQHandler(void);
//void FDCAN1_IT1_IRQHandler(void);

/********************************* Global variables *******************************/

/********************************* Implementations ********************************/

void CAN_Initialize(void)
{   
    //CAN initialization code to be added here

    CanWakeUp();
    CanEnterInitMode();
    CanInitClock();
    
    /* Enable configuration change */
    SET_BIT(FDCAN1->CCCR, FDCAN_CCCR_CCE);

    /* Set the no automatic retransmission */
    //SET_BIT(FDCAN1->CCCR, FDCAN_CCCR_DAR);

    /* Set the transmit pause feature */
    CLEAR_BIT(FDCAN1->CCCR, FDCAN_CCCR_TXP);

    /* Set the Protocol Exception Handling */
    SET_BIT(FDCAN1->CCCR, FDCAN_CCCR_PXHD);

    /* Set FDCAN Frame Format */
    MODIFY_REG(FDCAN1->CCCR, FDCAN_FRAME_FD_NO_BRS, FDCAN_FRAME_CLASSIC);
}

static void CanEnterInitMode(void)
{
    SET_BIT(FDCAN1->CCCR, FDCAN_CCCR_INIT);
    while ((FDCAN1->CCCR & FDCAN_CCCR_INIT) == 0);
}

static void CanLeaveInitMode(void)
{
    CLEAR_BIT(FDCAN1->CCCR, FDCAN_CCCR_INIT);
    while ((FDCAN1->CCCR & FDCAN_CCCR_INIT) == FDCAN_CCCR_INIT);
}

static void CanWakeUp(void)
{
    CLEAR_BIT(FDCAN1->CCCR, FDCAN_CCCR_CSR);
    while ((FDCAN1->CCCR & FDCAN_CCCR_CSA) == FDCAN_CCCR_CSA);
}

static void CanSleep(void)
{
    SET_BIT(FDCAN1->CCCR, FDCAN_CCCR_CSR);
    while ((FDCAN1->CCCR & FDCAN_CCCR_CSA) == 0);
}

static void CanInitClock(void)
{
    /** Use more stable external clock */
    LL_RCC_SetFDCANClockSource(LL_RCC_FDCAN_CLKSOURCE_HSE);

    /** Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_FDCAN);
}

static void CanOperatingModeSet(uint8_t mode)
{
    MODIFY_REG(FDCAN1->CCCR, FDCAN_CCCR_INIT, (mode << FDCAN_CCCR_INIT_Pos));
}

/*EOF*/
