/**
 * @file can_common.h
 * @brief CAN interface
  * 
 * @author Phuc VU
 * @date Jun 28, 2026
 */

#ifndef __CAN_COMMON_H_
#define __CAN_COMMON_H_

/********************************* Includes ***************************************/
#include <stdint.h>
#include <stdbool.h>

#include <stm32g4xx.h>

#ifdef __cplusplus
extern "C" {
#endif

/********************************* Constants definition ***************************/

#define FDCAN_TIMEOUT_VALUE 10U

#define FDCAN_TX_EVENT_FIFO_MASK (FDCAN_IR_TEFL | FDCAN_IR_TEFF | FDCAN_IR_TEFN)
#define FDCAN_RX_FIFO0_MASK (FDCAN_IR_RF0L | FDCAN_IR_RF0F | FDCAN_IR_RF0N)
#define FDCAN_RX_FIFO1_MASK (FDCAN_IR_RF1L | FDCAN_IR_RF1F | FDCAN_IR_RF1N)
#define FDCAN_ERROR_MASK (FDCAN_IR_ELO | FDCAN_IR_WDI | FDCAN_IR_PEA | FDCAN_IR_PED | FDCAN_IR_ARA)
#define FDCAN_ERROR_STATUS_MASK (FDCAN_IR_EP | FDCAN_IR_EW | FDCAN_IR_BO)

#define FDCAN_ELEMENT_MASK_STDID ((uint32_t)0x1FFC0000U) /* Standard Identifier         */
#define FDCAN_ELEMENT_MASK_EXTID ((uint32_t)0x1FFFFFFFU) /* Extended Identifier         */
#define FDCAN_ELEMENT_MASK_RTR   ((uint32_t)0x20000000U) /* Remote Transmission Request */
#define FDCAN_ELEMENT_MASK_XTD   ((uint32_t)0x40000000U) /* Extended Identifier         */
#define FDCAN_ELEMENT_MASK_ESI   ((uint32_t)0x80000000U) /* Error State Indicator       */
#define FDCAN_ELEMENT_MASK_TS    ((uint32_t)0x0000FFFFU) /* Timestamp                   */
#define FDCAN_ELEMENT_MASK_DLC   ((uint32_t)0x000F0000U) /* Data Length Code            */
#define FDCAN_ELEMENT_MASK_BRS   ((uint32_t)0x00100000U) /* Bit Rate Switch             */
#define FDCAN_ELEMENT_MASK_FDF   ((uint32_t)0x00200000U) /* FD Format                   */
#define FDCAN_ELEMENT_MASK_EFC   ((uint32_t)0x00800000U) /* Event FIFO Control          */
#define FDCAN_ELEMENT_MASK_MM    ((uint32_t)0xFF000000U) /* Message Marker              */
#define FDCAN_ELEMENT_MASK_FIDX  ((uint32_t)0x7F000000U) /* Filter Index                */
#define FDCAN_ELEMENT_MASK_ANMF  ((uint32_t)0x80000000U) /* Accepted Non-matching Frame */
#define FDCAN_ELEMENT_MASK_ET    ((uint32_t)0x00C00000U) /* Event type                  */

#define SRAMCAN_FLS_NBR                  (28U)         /* Max. Filter List Standard Number      */
#define SRAMCAN_FLE_NBR                  ( 8U)         /* Max. Filter List Extended Number      */
#define SRAMCAN_RF0_NBR                  ( 3U)         /* RX FIFO 0 Elements Number             */
#define SRAMCAN_RF1_NBR                  ( 3U)         /* RX FIFO 1 Elements Number             */
#define SRAMCAN_TEF_NBR                  ( 3U)         /* TX Event FIFO Elements Number         */
#define SRAMCAN_TFQ_NBR                  ( 3U)         /* TX FIFO/Queue Elements Number         */

#define SRAMCAN_FLS_SIZE            ( 1U * 4U)         /* Filter Standard Element Size in bytes */
#define SRAMCAN_FLE_SIZE            ( 2U * 4U)         /* Filter Extended Element Size in bytes */
#define SRAMCAN_RF0_SIZE            (18U * 4U)         /* RX FIFO 0 Elements Size in bytes      */
#define SRAMCAN_RF1_SIZE            (18U * 4U)         /* RX FIFO 1 Elements Size in bytes      */
#define SRAMCAN_TEF_SIZE            ( 2U * 4U)         /* TX Event FIFO Elements Size in bytes  */
#define SRAMCAN_TFQ_SIZE            (18U * 4U)         /* TX FIFO/Queue Elements Size in bytes  */

#define SRAMCAN_FLSSA ((uint32_t)0)                                                      /* Filter List Standard Start
                                                                                            Address                  */
#define SRAMCAN_FLESA ((uint32_t)(SRAMCAN_FLSSA + (SRAMCAN_FLS_NBR * SRAMCAN_FLS_SIZE))) /* Filter List Extended Start
                                                                                            Address                  */
#define SRAMCAN_RF0SA ((uint32_t)(SRAMCAN_FLESA + (SRAMCAN_FLE_NBR * SRAMCAN_FLE_SIZE))) /* Rx FIFO 0 Start Address  */
#define SRAMCAN_RF1SA ((uint32_t)(SRAMCAN_RF0SA + (SRAMCAN_RF0_NBR * SRAMCAN_RF0_SIZE))) /* Rx FIFO 1 Start Address  */
#define SRAMCAN_TEFSA ((uint32_t)(SRAMCAN_RF1SA + (SRAMCAN_RF1_NBR * SRAMCAN_RF1_SIZE))) /* Tx Event FIFO Start
                                                                                            Address */
#define SRAMCAN_TFQSA ((uint32_t)(SRAMCAN_TEFSA + (SRAMCAN_TEF_NBR * SRAMCAN_TEF_SIZE))) /* Tx FIFO/Queue Start
                                                                                            Address                  */
#define SRAMCAN_SIZE  ((uint32_t)(SRAMCAN_TFQSA + (SRAMCAN_TFQ_NBR * SRAMCAN_TFQ_SIZE))) /* Message RAM size         */


#define FDCAN_STANDARD_ID ((uint32_t)0x00000000U) /*!< Standard ID element */
#define FDCAN_EXTENDED_ID ((uint32_t)0x40000000U) /*!< Extended ID element */

#define FDCAN_DATA_FRAME   ((uint32_t)0x00000000U)  /*!< Data frame   */
#define FDCAN_REMOTE_FRAME ((uint32_t)0x20000000U)  /*!< Remote frame */

/********************************* Macros definition ******************************/

/**
  * @brief  Clear the specified FDCAN flags.
  * @param  __HANDLE__ FDCAN handle.
  * @param  __FLAG__ specifies the flags to clear.
  *         This parameter can be any combination of @arg FDCAN_flags
  * @retval None
  */
#define __HAL_FDCAN_CLEAR_FLAG(__HANDLE__, __FLAG__)             \
  ((__HANDLE__)->IR) = (__FLAG__)

/********************************* Types definition *******************************/

/**
  * @brief  FDCAN Message RAM blocks
  */
typedef struct
{
  uint32_t StandardFilterSA; /*!< Specifies the Standard Filter List Start Address.
                                  This parameter must be a 32-bit word address      */

  uint32_t ExtendedFilterSA; /*!< Specifies the Extended Filter List Start Address.
                                  This parameter must be a 32-bit word address      */

  uint32_t RxFIFO0SA;        /*!< Specifies the Rx FIFO 0 Start Address.
                                  This parameter must be a 32-bit word address      */

  uint32_t RxFIFO1SA;        /*!< Specifies the Rx FIFO 1 Start Address.
                                  This parameter must be a 32-bit word address      */

  uint32_t TxEventFIFOSA;    /*!< Specifies the Tx Event FIFO Start Address.
                                  This parameter must be a 32-bit word address      */

  uint32_t TxFIFOQSA;        /*!< Specifies the Tx FIFO/Queue Start Address.
                                  This parameter must be a 32-bit word address      */

} FDCAN_MsgRamAddressTypeDef;

/**
  * @brief  FDCAN Rx header structure definition
  */
typedef struct
{
  uint32_t Identifier;            /*!< Specifies the identifier.
                                       This parameter must be a number between:
                                        - 0 and 0x7FF, if IdType is FDCAN_STANDARD_ID
                                        - 0 and 0x1FFFFFFF, if IdType is FDCAN_EXTENDED_ID               */

  uint32_t IdType;                /*!< Specifies the identifier type of the received message.
                                       This parameter can be a value of @ref FDCAN_id_type               */

  uint32_t RxFrameType;           /*!< Specifies the the received message frame type.
                                       This parameter can be a value of @ref FDCAN_frame_type            */

  uint32_t DataLength;            /*!< Specifies the received frame length.
                                        This parameter can be a value of @ref FDCAN_data_length_code     */

  uint32_t ErrorStateIndicator;   /*!< Specifies the error state indicator.
                                       This parameter can be a value of @ref FDCAN_error_state_indicator */

  uint32_t BitRateSwitch;         /*!< Specifies whether the Rx frame is received with or without bit
                                       rate switching.
                                       This parameter can be a value of @ref FDCAN_bit_rate_switching    */

  uint32_t FDFormat;              /*!< Specifies whether the Rx frame is received in classic or FD
                                       format.
                                       This parameter can be a value of @ref FDCAN_format                */

  uint32_t RxTimestamp;           /*!< Specifies the timestamp counter value captured on start of frame
                                       reception.
                                       This parameter must be a number between 0 and 0xFFFF              */

  uint32_t FilterIndex;           /*!< Specifies the index of matching Rx acceptance filter element.
                                       This parameter must be a number between:
                                        - 0 and (SRAMCAN_FLS_NBR-1), if IdType is FDCAN_STANDARD_ID
                                        - 0 and (SRAMCAN_FLE_NBR-1), if IdType is FDCAN_EXTENDED_ID
                                       When the frame is a Non-Filter matching frame, this parameter
                                       is unused.                                                        */

  uint32_t IsFilterMatchingFrame; /*!< Specifies whether the accepted frame did not match any Rx filter.
                                       Acceptance of non-matching frames may be enabled via
                                       HAL_FDCAN_ConfigGlobalFilter().
                                       This parameter takes 0 if the frame matched an Rx filter or
                                       1 if it did not match any Rx filter                               */

} FDCAN_RxHeaderTypeDef;

/********************************* Global variable ********************************/
/********************************* API functions prototype ************************/



/* p.1971 RM0440 - CCCR */
static inline void FDCAN_EnterInitMode(FDCAN_GlobalTypeDef * hFDCAN)
{
    SET_BIT(hFDCAN->CCCR, FDCAN_CCCR_INIT);
    while ((hFDCAN->CCCR & FDCAN_CCCR_INIT) == 0);
}

static inline void FDCAN_LeaveInitMode(FDCAN_GlobalTypeDef * hFDCAN)
{
    CLEAR_BIT(hFDCAN->CCCR, FDCAN_CCCR_INIT);
    while ((hFDCAN->CCCR & FDCAN_CCCR_INIT) == FDCAN_CCCR_INIT);
}

static inline void FDCAN_WakeUp(FDCAN_GlobalTypeDef * hFDCAN)
{
    CLEAR_BIT(hFDCAN->CCCR, FDCAN_CCCR_CSR);
    while ((hFDCAN->CCCR & FDCAN_CCCR_CSA) == FDCAN_CCCR_CSA);
}

static inline void FDCAN_Sleep(FDCAN_GlobalTypeDef * hFDCAN)
{
    SET_BIT(hFDCAN->CCCR, FDCAN_CCCR_CSR);
    while ((hFDCAN->CCCR & FDCAN_CCCR_CSA) == 0);
}

static inline void FDCAN_DisablePauseTransmit(FDCAN_GlobalTypeDef * hFDCAN)
{
    CLEAR_BIT(hFDCAN->CCCR, FDCAN_CCCR_TXP);
}

static inline void FDCAN_EnablePauseTransmit(FDCAN_GlobalTypeDef * hFDCAN)
{
    SET_BIT(hFDCAN->CCCR, FDCAN_CCCR_TXP);
}

static inline void FDCAN_DisableAutoRetransmission(FDCAN_GlobalTypeDef * hFDCAN)
{
    SET_BIT(FDCAN1->CCCR, FDCAN_CCCR_DAR);
}

static inline void FDCAN_EnableAutoRetransmission(FDCAN_GlobalTypeDef * hFDCAN)
{
    CLEAR_BIT(FDCAN1->CCCR, FDCAN_CCCR_DAR);
}

static inline void FDCAN_EnableProtocolExceptionHandling(FDCAN_GlobalTypeDef * hFDCAN)
{
    CLEAR_BIT(FDCAN1->CCCR, FDCAN_CCCR_PXHD);
}

static inline void FDCAN_DisableProtocolExceptionHandling(FDCAN_GlobalTypeDef * hFDCAN)
{
    SET_BIT(FDCAN1->CCCR, FDCAN_CCCR_PXHD);
}


#ifdef __cplusplus
}
#endif

#endif /* __CAN_COMMON_H_ */