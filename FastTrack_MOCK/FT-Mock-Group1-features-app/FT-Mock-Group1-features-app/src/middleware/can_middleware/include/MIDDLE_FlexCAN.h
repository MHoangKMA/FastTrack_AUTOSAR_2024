/*
 * MIDDLE_FLexCAN.h
 *
 *  Created on: Nov 4, 2024
 *      Author: adm
 */

#ifndef INCLUDE_MIDDLE_FLEXCAN_H_
#define INCLUDE_MIDDLE_FLEXCAN_H_

#include "FlexCAN.h"

/*==================================================================================================
*                                        ENUMS
==================================================================================================*/

/**
 * @brief Enumeration of FlexCAN message buffer (MB) types and error types.
 *
 * Defines various types of handlers for different message buffers (MB0 to MB31) and error/ORed handlers.
 */
typedef enum
{
    /* Defines for MBx callback */
    MIDDLE_HANDLER_MB_0_TYPE      = 0U,
    MIDDLE_HANDLER_MB_1_TYPE,
    MIDDLE_HANDLER_MB_2_TYPE,
    MIDDLE_HANDLER_MB_3_TYPE,
    MIDDLE_HANDLER_MB_4_TYPE,
    MIDDLE_HANDLER_MB_5_TYPE,
    MIDDLE_HANDLER_MB_6_TYPE,
    MIDDLE_HANDLER_MB_7_TYPE,
    MIDDLE_HANDLER_MB_8_TYPE,
    MIDDLE_HANDLER_MB_9_TYPE,
    MIDDLE_HANDLER_MB_10_TYPE,
    MIDDLE_HANDLER_MB_11_TYPE,
    MIDDLE_HANDLER_MB_12_TYPE,
    MIDDLE_HANDLER_MB_13_TYPE,
    MIDDLE_HANDLER_MB_14_TYPE,
    MIDDLE_HANDLER_MB_15_TYPE,
    MIDDLE_HANDLER_MB_16_TYPE,
    MIDDLE_HANDLER_MB_17_TYPE,
    MIDDLE_HANDLER_MB_18_TYPE,
    MIDDLE_HANDLER_MB_19_TYPE,
    MIDDLE_HANDLER_MB_20_TYPE,
    MIDDLE_HANDLER_MB_21_TYPE,
    MIDDLE_HANDLER_MB_22_TYPE,
    MIDDLE_HANDLER_MB_23_TYPE,
    MIDDLE_HANDLER_MB_24_TYPE,
    MIDDLE_HANDLER_MB_25_TYPE,
    MIDDLE_HANDLER_MB_26_TYPE,
    MIDDLE_HANDLER_MB_27_TYPE,
    MIDDLE_HANDLER_MB_28_TYPE,
    MIDDLE_HANDLER_MB_29_TYPE,
    MIDDLE_HANDLER_MB_30_TYPE,
    MIDDLE_HANDLER_MB_31_TYPE,

    /* Defines for ERROR and ORED callback */
    MIDDLE_HANDLER_ERROR_TYPE,
    MIDDLE_HANDLER_ORED_TYPE
} MID_CAN_Handler_e;

/**
 * @brief Enumeration for selecting the FlexCAN hardware module.
 *
 * This enum allows selecting one of the available FlexCAN modules (0, 1, or 2).
 */
typedef enum
{
    MODULE_0_INS  = FlexCAN0_INS,  /*!< FlexCAN Module 0 instance */
    MODULE_1_INS  = FlexCAN1_INS,  /*!< FlexCAN Module 1 instance */
    MODULE_2_INS  = FlexCAN2_INS   /*!< FlexCAN Module 2 instance */
} MID_CAN_ModuleIns_e;

/*==================================================================================================
*                                       STRUCTURES
==================================================================================================*/

/**
 * @brief Structure to hold user-specific configuration for FlexCAN message buffer.
 *
 * This structure contains the index of the message buffer, message ID, interrupt enable flag, 
 * callback function, and handler type for the message buffer.
 */
typedef struct
{
    FlexCAN_MbIndex_e            MbIndex;      /*!< Message buffer index */
    uint32_t                     MbID;         /*!< Message buffer ID */
    bool                         MbInt;        /*!< Message buffer interrupt enable flag */
    FlexCAN_CallbackType         HandlerFunc;  /*!< Callback function for message buffer */
    MID_CAN_Handler_e            HandlerType;  /*!< Handler type (e.g., message buffer or error handler) */
} MID_CAN_UserConfigType;

/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/

/**
 * @brief  Initializes the specified FlexCAN module.
 *
 * @param[in]  Ins  The FlexCAN module instance to initialize (e.g., MODULE_0_INS).
 */
void MID_CAN_Init(MID_CAN_ModuleIns_e Ins);

/**
 * @brief  Deinitializes the specified FlexCAN module.
 *
 * @param[in]  Ins  The FlexCAN module instance to deinitialize (e.g., MODULE_0_INS).
 */
void MID_CAN_DeInit(MID_CAN_ModuleIns_e Ins);

/**
 * @brief  Sets the callback function for a specific FlexCAN module and message buffer.
 *
 * @param[in]  Ins        The FlexCAN module instance.
 * @param[in]  UserConfig Pointer to user configuration structure defining the callback and message buffer details.
 */
void MID_CAN_SetCallback(MID_CAN_ModuleIns_e Ins, MID_CAN_UserConfigType *UserConfig);

/**
 * @brief  Initializes the specified message buffer for standard transmit mode.
 *
 * @param[in]  Ins        The FlexCAN module instance.
 * @param[in]  UserConfig Pointer to user configuration structure defining the message buffer parameters.
 */
void MID_CAN_StdTxMbInit(MID_CAN_ModuleIns_e Ins, MID_CAN_UserConfigType *UserConfig);

/**
 * @brief  Initializes the specified message buffer for standard receive mode.
 *
 * @param[in]  Ins        The FlexCAN module instance.
 * @param[in]  UserConfig Pointer to user configuration structure defining the message buffer parameters.
 */
void MID_CAN_StdRxMbInit(MID_CAN_ModuleIns_e Ins, MID_CAN_UserConfigType *UserConfig);

/**
 * @brief  Transmits data over the specified message buffer.
 *
 * @param[in]  Ins        The FlexCAN module instance.
 * @param[in]  MbIndex   The message buffer index for transmission.
 * @param[in]  TxBuffer  Pointer to the transmit data buffer.
 */
void MID_CAN_Transmit(MID_CAN_ModuleIns_e Ins, FlexCAN_MbIndex_e MbIndex, uint8_t *TxBuffer);

/**
 * @brief  Receives data over the specified message buffer.
 *
 * @param[in]  Ins        The FlexCAN module instance.
 * @param[in]  MbIndex   The message buffer index for reception.
 * @param[out] RxBuffer  Pointer to the receive data buffer.
 */
void MID_CAN_Receive(MID_CAN_ModuleIns_e Ins, FlexCAN_MbIndex_e MbIndex, uint8_t *RxBuffer);

/**
 * @brief  Gets the acknowledgment status of the FlexCAN module.
 *
 * @param[in]  Ins  The FlexCAN module instance.
 *
 * @return uint8_t  The acknowledgment status (1 for acknowledged, 0 for not).
 */
uint8_t MID_CAN_GetAckStatus(MID_CAN_ModuleIns_e Ins);

#endif /* INCLUDE_MIDDLE_FLEXCAN_H_ */
