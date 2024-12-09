/*
 * FlexCAN.h
 *
 *  Created on: Oct 23, 2024
 *      Author: Le Hung
 */

#ifndef FLEXCAN_H_
#define FLEXCAN_H_

#include "Driver_Header.h"

/* ------------------------------------------------------------------------------------------------------------------------------------------------------
   -- Definitions
   ------------------------------------------------------------------------------------------------------------------------------------------------------ */
#define NUMBER_OF_HANDLER_TYPE						4U

#define	MB_FLEXCAN_0								((FlexCAN_MbType*)0x40024080U)
#define MB_FLEXCAN_1								((FlexCAN_MbType*)0x40025080U)
#define MB_FLEXCAN_2								((FlexCAN_MbType*)0x4002B080U)

#define FLEXCAN_RAMn_DATA_WORD_0_EDL_MASK			(0x80000000U)
#define FLEXCAN_RAMn_DATA_WORD_0_EDL_SHIFT			(31U)
#define FLEXCAN_RAMn_DATA_WORD_0_EDL(x)     		(((uint32_t)(((uint32_t)(x)) << FLEXCAN_RAMn_DATA_WORD_0_EDL_SHIFT)) & FLEXCAN_RAMn_DATA_WORD_0_EDL_MASK)

#define FLEXCAN_RAMn_DATA_WORD_0_BRS_MASK			(0x40000000U)
#define FLEXCAN_RAMn_DATA_WORD_0_BRS_SHIFT			(30U)
#define FLEXCAN_RAMn_DATA_WORD_0_BRS(x)     		(((uint32_t)(((uint32_t)(x)) << FLEXCAN_RAMn_DATA_WORD_0_BRS_SHIFT)) & FLEXCAN_RAMn_DATA_WORD_0_BRS_MASK)

#define FLEXCAN_RAMn_DATA_WORD_0_ESI_MASK			(0x20000000U)
#define FLEXCAN_RAMn_DATA_WORD_0_ESI_SHIFT			(29U)
#define FLEXCAN_RAMn_DATA_WORD_0_ESI(x)     		(((uint32_t)(((uint32_t)(x)) << FLEXCAN_RAMn_DATA_WORD_0_ESI_SHIFT)) & FLEXCAN_RAMn_DATA_WORD_0_ESI_MASK)

#define FLEXCAN_RAMn_DATA_WORD_0_CODE_MASK			(0xF000000U)
#define FLEXCAN_RAMn_DATA_WORD_0_CODE_SHIFT			(24U)
#define FLEXCAN_RAMn_DATA_WORD_0_CODE(x)    		(((uint32_t)(((uint32_t)(x)) << FLEXCAN_RAMn_DATA_WORD_0_CODE_SHIFT)) & FLEXCAN_RAMn_DATA_WORD_0_CODE_MASK)

#define FLEXCAN_RAMn_DATA_WORD_0_SRR_MASK   		(0x400000U)
#define FLEXCAN_RAMn_DATA_WORD_0_SRR_SHIFT  		(22U)
#define FLEXCAN_RAMn_DATA_WORD_0_SRR(x)     		(((uint32_t)(((uint32_t)(x)) << FLEXCAN_RAMn_DATA_WORD_0_SRR_SHIFT)) & FLEXCAN_RAMn_DATA_WORD_0_SRR_MASK)

#define FLEXCAN_RAMn_DATA_WORD_0_IDE_MASK   		(0x200000U)
#define FLEXCAN_RAMn_DATA_WORD_0_IDE_SHIFT  		(21U)
#define FLEXCAN_RAMn_DATA_WORD_0_IDE(x)     		(((uint32_t)(((uint32_t)(x)) << FLEXCAN_RAMn_DATA_WORD_0_IDE_SHIFT)) & FLEXCAN_RAMn_DATA_WORD_0_IDE_MASK)

#define FLEXCAN_RAMn_DATA_WORD_0_RTR_MASK   		(0x100000U)
#define FLEXCAN_RAMn_DATA_WORD_0_RTR_SHIFT  		(20U)
#define FLEXCAN_RAMn_DATA_WORD_0_RTR(x)     		(((uint32_t)(((uint32_t)(x)) << FLEXCAN_RAMn_DATA_WORD_0_RTR_SHIFT)) & FLEXCAN_RAMn_DATA_WORD_0_RTR_MASK)

#define FLEXCAN_RAMn_DATA_WORD_0_DLC_MASK   		(0xF0000U)
#define FLEXCAN_RAMn_DATA_WORD_0_DLC_SHIFT  		(16U)
#define FLEXCAN_RAMn_DATA_WORD_0_DLC(x)     		(((uint32_t)(((uint32_t)(x)) << FLEXCAN_RAMn_DATA_WORD_0_DLC_SHIFT)) & FLEXCAN_RAMn_DATA_WORD_0_DLC_MASK)

#define FLEXCAN_RAMn_DATA_WORD_0_TIME_STAMP_MASK   	(0x7FFFU)
#define FLEXCAN_RAMn_DATA_WORD_0_TIME_STAMP_SHIFT  	(0U)
#define FLEXCAN_RAMn_DATA_WORD_0_TIME_STAMP(x)     	(((uint32_t)(((uint32_t)(x)) << FLEXCAN_RAMn_DATA_WORD_0_TIME_STAMP_SHIFT)) & FLEXCAN_RAMn_DATA_WORD_0_TIME_STAMP_MASK)

#define FLEXCAN_RAMn_DATA_WORD_1_ID_MASK    		(0x1FFC0000U)
#define FLEXCAN_RAMn_DATA_WORD_1_ID_SHIFT   		(18U)
#define FLEXCAN_RAMn_DATA_WORD_1_ID(x)      		(((uint32_t)(((uint32_t)(x)) << FLEXCAN_RAMn_DATA_WORD_1_ID_SHIFT)) & FLEXCAN_RAMn_DATA_WORD_1_ID_MASK)


/* ------------------------------------------------------------------------------------------------------------------------------------------------------
   -- Typedefs
   ------------------------------------------------------------------------------------------------------------------------------------------------------ */

typedef void (*FlexCAN_CallbackType)(void);

/**
 * @brief Enum type for Adc function return type
 */
typedef enum
{
    FLEXCAN_DRIVER_RETURN_CODE_ERROR     = 0U,
    FLEXCAN_DRIVER_RETURN_CODE_SUCCESSED = 1U
}FlexCAN_Driver_ReturnCode_e;

/* Structures and enums for CAN configuration */
typedef enum
{
    FlexCAN0_INS = 0U,
    FlexCAN1_INS = 1U,
    FlexCAN2_INS = 2U,
}FlexCAN_Instance_e;


/**
 * @brief Enum type for FlexCAN Pin Tx
 */
typedef enum
{
	FlexCAN0_Tx	= 5U,		/*!< FlexCAN0 Tx Pin */
	FlexCAN1_Tx	= 7U,		/*!< FlexCAN1 Tx Pin */
	FlexCAN2_Tx	= 13U		/*!< FlexCAN2 Tx Pin */
}FlexCAN_TxPin_e;

/**
 * @brief Enum type for FlexCAN Pin Rx
 */
typedef enum
{
	FlexCAN0_Rx	= 4U,		/*!< FlexCAN0 Rx Pin */
	FlexCAN1_Rx	= 6U,		/*!< FlexCAN1 Rx Pin */
	FlexCAN2_Rx	= 12U		/*!< FlexCAN2 Rx Pin */
}FlexCAN_RxPin_e;

/**
 * @brief Enum type for FlexCAN mode
 */
typedef enum
{
	FlexCAN_MODE_NORMAL 	= 0U,	/*!< FlexCAN normal mode */
	FlexCAN_MODE_LOOPBACK	= 1U		/*!< FlexCAN loopback mode */
}FlexCAN_Mode_e;

/**
 * @brief Enum type for FlexCAN clock source
 */
typedef enum
{
    FlexCAN_CLKSRC_SYS = 1U,    /*!< System clock source */
    FlexCAN_CLKSRC_OSC = 0U     /*!< Oscillator clock source */
} FlexCAN_ClkSrc_e;

/**
 * @brief Enum type for FlexCAN message buffer type
 */
typedef enum
{
    FlexCAN_MB_TX = 0U,         /*!< Transmit message buffer */
    FlexCAN_MB_RX = 1U          /*!< Receive message buffer */
} FlexCAN_MbType_e;

/**
 * @brief Enum type for FlexCAN bus-off interrupt control
 */
typedef enum
{
    FlexCAN_INT_BUSOFF_DISABLE = 0U, /*!< Bus-off interrupt disabled */
    FlexCAN_INT_BUSOFF_ENABLE = 1U   /*!< Bus-off interrupt enabled */
} FlexCAN_IntBusOff_e;

/**
 * @brief Enum type for FlexCAN error interrupt control
 */
typedef enum
{
    FlexCAN_INT_ERROR_DISABLE = 0U,  /*!< Error interrupt disabled */
    FlexCAN_INT_ERROR_ENABLE = 1U    /*!< Error interrupt enabled */
} FlexCAN_IntError_e;

/**
 * @brief Enum type for FlexCAN transmit warning interrupt control
 */
typedef enum
{
    FlexCAN_INT_TxWARNING_DISABLE = 0U, /*!< Transmit warning interrupt disabled */
    FlexCAN_INT_TxWARNING_ENABLE = 1U   /*!< Transmit warning interrupt enabled */
} FlexCAN_IntTxWarning_e;

/**
 * @brief Enum type for FlexCAN receive warning interrupt control
 */
typedef enum
{
    FlexCAN_INT_RxWARNING_DISABLE = 0U, /*!< Receive warning interrupt disabled */
    FlexCAN_INT_RxWARNING_ENABLE = 1U   /*!< Receive warning interrupt enabled */
} FlexCAN_IntRxWarning_e;

typedef struct
{
    FlexCAN_IntBusOff_e     IntBusOff;   /*!< Bus-off interrupt control */
    FlexCAN_IntError_e      IntError;    /*!< Error interrupt control */
    FlexCAN_IntTxWarning_e  IntTxWarning;/*!< Transmit warning interrupt control */
    FlexCAN_IntRxWarning_e  IntRxWarning;/*!< Receive warning interrupt control */
} FlexCAN_InterruptType;

typedef enum
{
    Rx_CODE_INACTIVE    = 0U, /*!< Receive code inactive */
    Rx_CODE_EMPTY       = 4U, /*!< Receive code empty */
    Rx_CODE_FULL        = 2U, /*!< Receive code full */
    Rx_CODE_OVERRUN     = 6U, /*!< Receive code overrun */
    Rx_CODE_RANSWER     = 10U,/*!< Receive code remote answer */
    Rx_CODE_BUSY        = 1U  /*!< Receive code busy */
} FlexCAN_RxCODE_e;

typedef enum
{
    Tx_CODE_INACTIVE    = 8U, /*!< Transmit code inactive */
    Tx_CODE_ABORT       = 9U, /*!< Transmit code abort */
    Tx_CODE_DATA        = 12U,/*!< Transmit code data */
    Tx_CODE_REMOTE      = 12U,/*!< Transmit code remote */
    Tx_CODE_TANSWER     = 14U /*!< Transmit code answer */
} FlexCAN_TxCODE_e;

typedef enum
{
    FlexCAN_STATUS_FLAG_ERRINT        = 1U, /*!< Error interrupt flag */
    FlexCAN_STATUS_FLAG_BOFFINT       = 2U, /*!< Bus-off interrupt flag */
    FlexCAN_STATUS_FLAG_RX            = 3U, /*!< Receive flag */
    FlexCAN_STATUS_FLAG_FLTCONF       = 4U, /*!< Fault confinement flag */
    FlexCAN_STATUS_FLAG_TX            = 6U, /*!< Transmit flag */
    FlexCAN_STATUS_FLAG_IDLE          = 7U, /*!< Idle flag */
    FlexCAN_STATUS_FLAG_RXWRN         = 8U, /*!< Receive warning flag */
    FlexCAN_STATUS_FLAG_TXWRN         = 9U, /*!< Transmit warning flag */
    FlexCAN_STATUS_FLAG_STFERR        = 10U,/*!< Stuff error flag */
    FlexCAN_STATUS_FLAG_FRMERR        = 11U,/*!< Form error flag */
    FlexCAN_STATUS_FLAG_CRCERR        = 12U,/*!< CRC error flag */
    FlexCAN_STATUS_FLAG_ACKERR        = 13U,/*!< Acknowledge error flag */
    FlexCAN_STATUS_FLAG_BIT0ERR       = 14U,/*!< Bit0 error flag */
    FlexCAN_STATUS_FLAG_BIT1ERR       = 15U,/*!< Bit1 error flag */
    FlexCAN_STATUS_FLAG_RWRNINT       = 16U,/*!< Receive warning interrupt flag */
    FlexCAN_STATUS_FLAG_TWRNINT       = 17U,/*!< Transmit warning interrupt flag */
    FlexCAN_STATUS_FLAG_SYNCH         = 18U,/*!< Synchronization flag */
    FlexCAN_STATUS_FLAG_BOFFDONEINT   = 19U,/*!< Bus-off done interrupt flag */
    FlexCAN_STATUS_FLAG_ERRINT_FAST   = 20U,/*!< Fast error interrupt flag */
    FlexCAN_STATUS_FLAG_ERROVR        = 21U,/*!< Error overrun flag */
    FlexCAN_STATUS_FLAG_STFERR_FAST   = 26U,/*!< Fast stuff error flag */
    FlexCAN_STATUS_FLAG_FRMERR_FAST   = 27U,/*!< Fast form error flag */
    FlexCAN_STATUS_FLAG_CRCERR_FAST   = 28U,/*!< Fast CRC error flag */
    FlexCAN_STATUS_FLAG_BIT0ERR_FAST  = 30U,/*!< Fast bit0 error flag */
    FlexCAN_STATUS_FLAG_BIT1ERR_FAST  = 31U /*!< Fast bit1 error flag */
} FlexCAN_StatusFlag_e;

typedef enum
{
    MB0 = 0U, /*!< Message Buffer 0 */
    MB1,      /*!< Message Buffer 1 */
    MB2,      /*!< Message Buffer 2 */
    MB3,      /*!< Message Buffer 3 */
    MB4,      /*!< Message Buffer 4 */
    MB5,      /*!< Message Buffer 5 */
    MB6,      /*!< Message Buffer 6 */
    MB7,      /*!< Message Buffer 7 */
    MB8,      /*!< Message Buffer 8 */
    MB9,      /*!< Message Buffer 9 */
    MB10,     /*!< Message Buffer 10 */
    MB11,     /*!< Message Buffer 11 */
    MB12,     /*!< Message Buffer 12 */
    MB13,     /*!< Message Buffer 13 */
    MB14,     /*!< Message Buffer 14 */
    MB15,     /*!< Message Buffer 15 */
    MB16,     /*!< Message Buffer 16 */
    MB17,     /*!< Message Buffer 17 */
    MB18,     /*!< Message Buffer 18 */
    MB19,     /*!< Message Buffer 19 */
    MB20,     /*!< Message Buffer 20 */
    MB21,     /*!< Message Buffer 21 */
    MB22,     /*!< Message Buffer 22 */
    MB23,     /*!< Message Buffer 23 */
    MB24,     /*!< Message Buffer 24 */
    MB25,     /*!< Message Buffer 25 */
    MB26,     /*!< Message Buffer 26 */
    MB27,     /*!< Message Buffer 27 */
    MB28,     /*!< Message Buffer 28 */
    MB29,     /*!< Message Buffer 29 */
    MB30,     /*!< Message Buffer 30 */
    MB31      /*!< Message Buffer 31 */
} FlexCAN_MbIndex_e;

typedef enum
{
    FLEXCAN_STATE_UNINIT   = 0U, /*!< Uninitialized state */
    FLEXCAN_STATE_READY    = 1U, /*!< Ready state */
    FLEXCAN_STATE_STARTED  = 2U  /*!< Started state */
} FlexCAN_State_e;

typedef struct
{
    FlexCAN_TxPin_e    TxPin; /*!< Transmit Pin */
    FlexCAN_RxPin_e    RxPin; /*!< Receive Pin */
} FlexCAN_PinType;

typedef struct
{
    uint8_t             MaxNoMB;     /*!< Maximum number of message buffers */
    uint32_t            BitRate;     /*!< Bit rate for CAN communication */
    uint32_t            ClkFreq;     /*!< Clock frequency */
    FlexCAN_Mode_e      RunMode;     /*!< Operating mode */
    FlexCAN_ClkSrc_e    CLkSrc;      /*!< Clock source */
    FlexCAN_PinType     PortPin;     /*!< Port pin configuration */
    FlexCAN_InterruptType IntControl;/*!< Interrupt control configuration */
} FlexCAN_ConfigType;

typedef enum
{
    FlexCAN_STANDARD = 0U, /*!< Standard message ID format */
    FlexCAN_EXTENDED = 1U  /*!< Extended message ID format */
} FlexCAN_MsgIDType_e;

typedef struct
{
    uint8_t             EDL;        /*!< Extended data length */
    uint8_t             BRS;        /*!< Bit rate switch */
    uint8_t             ESI;        /*!< Error state indicator */
    FlexCAN_MbType_e    MbType;     /*!< Message buffer type */
    FlexCAN_MsgIDType_e IdType;     /*!< Message ID type */
    bool                IsRemote;   /*!< Remote transmission request */
    uint8_t             DataLen;    /*!< Data length */
    uint32_t            MbID;       /*!< Message buffer ID */
    bool                IsEnableMbInt; /*!< Enable message buffer interrupt */
} FlexCAN_MbHeaderType;

typedef struct
{
    uint32_t            Header[2];  /*!< Message header */
    uint32_t            Payload[2]; /*!< Message payload */
} FlexCAN_MbStructureType;

typedef struct
{
    FlexCAN_MbStructureType MB[32]; /*!< Array of message buffers */
} FlexCAN_MbType;

/* ------------------------------------------------------------------------------------------------------------------------------------------------------
   -- API
   ------------------------------------------------------------------------------------------------------------------------------------------------------ */
/**
 * @brief FlexCAN initialization function
 *
 * This function initializes FlexCAN module with given configuration.
 *
 * @param FlexCAN_Ins - FlexCAN instance number
 * @param FlexCAN_Config - pointer to FlexCAN configuration structure
 *
 * @return FlexCAN_Driver_ReturnCode_e - status of the operation
 */
FlexCAN_Driver_ReturnCode_e FlexCAN_Init(FlexCAN_Instance_e FlexCAN_Ins, FlexCAN_ConfigType *FlexCAN_Config);
/**
 * @brief De-initializes the FlexCAN module for the specified instance.
 *
 * @param FlexCAN_Ins - FlexCAN instance number
 * @return FlexCAN_Driver_ReturnCode_e - status of the operation
 */
FlexCAN_Driver_ReturnCode_e FlexCAN_DeInit(FlexCAN_Instance_e FlexCAN_Ins);

/**
 * @brief Transmits a message using the specified FlexCAN instance and message buffer index.
 *
 * @param FlexCAN_Ins - FlexCAN instance number
 * @param MbIndex - Message buffer index
 * @param MsgData - Pointer to message data to be transmitted
 * @return FlexCAN_Driver_ReturnCode_e - status of the operation
 */
FlexCAN_Driver_ReturnCode_e FlexCAN_Transmit(FlexCAN_Instance_e FlexCAN_Ins, FlexCAN_MbIndex_e MbIndex, uint8_t * MsgData);

/**
 * @brief Reads data from a specified mailbox in the FlexCAN module.
 *
 * @param FlexCAN_Ins - FlexCAN instance number
 * @param MbIndex - Message buffer index
 * @param MsgData - Pointer to buffer to store received message data
 * @return FlexCAN_Driver_ReturnCode_e - status of the operation
 */
FlexCAN_Driver_ReturnCode_e FlexCAN_ReadMailboxData(FlexCAN_Instance_e FlexCAN_Ins, FlexCAN_MbIndex_e MbIndex, uint8_t * MsgData);

/**
 * @brief Registers a callback function for the specified FlexCAN instance.
 *
 * @param Ins - FlexCAN instance number
 * @param CallbackFunc - Callback function to be registered
 * @param CallbackID - Identifier for the callback
 * @return FlexCAN_Driver_ReturnCode_e - status of the operation
 */
FlexCAN_Driver_ReturnCode_e FlexCAN_CallbackRegister(FlexCAN_Instance_e Ins, FlexCAN_CallbackType CallbackFunc, uint8_t CallbackID);

/**
 * @brief Gets the current state of the specified FlexCAN module.
 *
 * @param Ins - FlexCAN instance number
 * @return FlexCAN_State_e - current module state
 */
FlexCAN_State_e FlexCAN_GetModuleState(FlexCAN_Instance_e Ins);

/**
 * @brief Retrieves the status of a specific flag for the specified FlexCAN instance.
 *
 * @param Ins - FlexCAN instance number
 * @param FlagType - Type of status flag to retrieve
 * @return uint8_t - status of the specified flag
 */
uint8_t FlexCAN_GetStatusFlag(FlexCAN_Instance_e Ins, FlexCAN_StatusFlag_e FlagType);

#endif /* FLEXCAN_H_ */
