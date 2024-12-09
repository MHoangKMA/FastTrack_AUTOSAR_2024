/*
 * FlexCAN.c
 *
 *  Created on: Oct 23, 2024
 *      Author: adm
 */

#include "FlexCAN.h"
#include <stddef.h>

/* ----------------------------------------------------------------------------
   -- Definitions
   ---------------------------------------------------------------------------- */
#define NUMBER_OF_ERROR_ORED_HANDLER_TYPE	(FLEXCAN_INSTANCE_COUNT*2U)
#define ERROR_HANDLER_GAP			(2U)
#define ORED_HANDLER_GAP			(1U)
#define ERROR_INT                               (0x300002U)    /*!< Masks for ErrorOvr, ErrorFast, Error */
#define BUS_OFF_INT                             (0xB0004U)     /*!< Masks for busOff, Tx/Rx Warning */
#define NUMBER_OF_MB				(32U)
#define ERROR_CALLBACK_ID			(32U)

/* ----------------------------------------------------------------------------
   -- Variables
   ---------------------------------------------------------------------------- */
FlexCAN_CallbackType FlexCAN_Callback[NUMBER_OF_ERROR_ORED_HANDLER_TYPE] = { NULL };
FlexCAN_CallbackType FlexCAN_MbCallback[NUMBER_OF_MB] = { NULL };

FlexCAN_State_e	FlexCAN_CurrentState[FLEXCAN_INSTANCE_COUNT] = { FLEXCAN_STATE_UNINIT };

/**
 * Base addresses for FLEXCAN modules.
 * @note This array is indexed by LPUART instance numbers.
 */
static FLEXCAN_Type *FlexCAN_Base_Addr[FLEXCAN_INSTANCE_COUNT] = IP_FLEXCAN_BASE_PTRS;

FlexCAN_MbType * FlexCAN_MB[FLEXCAN_INSTANCE_COUNT] = {MB_FLEXCAN_0, MB_FLEXCAN_1, MB_FLEXCAN_2};

/* ----------------------------------------------------------------------------
   -- Private functions
   ---------------------------------------------------------------------------- */
static void FlexCAN_ModuleControl(FLEXCAN_Type *FlexCANx, uint8_t EnOrDis);
static void FlexCAN_ClkSrcSelect(FLEXCAN_Type *FlexCANx, FlexCAN_ClkSrc_e CLkSrc);
static void FlexCAN_ClearMB(FLEXCAN_Type *FlexCANx);
static void FlexCAN_RunModeSelect(FLEXCAN_Type *FlexCANx, FlexCAN_Mode_e Mode);
static void FlexCAN_IntControl(FLEXCAN_Type *FlexCANx, FlexCAN_InterruptType IntType);
static void FlexCAN_SetBitRate(FLEXCAN_Type *FlexCANx, uint32_t Clocks, uint32_t BitRate);
static void FLexCAN_FreezeModeControl(FLEXCAN_Type *FlexCANx, uint8_t EnOrDis);
static void FlexCAN_SoftReset(FLEXCAN_Type *FlexCANx);
static void FlexCAN_MBSetEDL(FlexCAN_MbStructureType * Mbx, uint8_t EDLValue);
static void FlexCAN_MBSetBRS(FlexCAN_MbStructureType * Mbx, uint8_t BRSValue);
static void FlexCAN_MBSetESI(FlexCAN_MbStructureType * Mbx, uint8_t ESIValue);
static void FlexCAN_MBSetIDType(FlexCAN_MbStructureType * Mbx, FlexCAN_MsgIDType_e IDType);
static void FlexCAN_MBSetRTR(FlexCAN_MbStructureType * Mbx, bool IsRemote);
static void FlexCAN_MBSetDataLength(FlexCAN_MbStructureType * Mbx, uint16_t Datalen);
static void FlexCAN_MBSetID(FlexCAN_MbStructureType * Mbx, uint16_t ID);
static void FlexCAN_MbSetType(FlexCAN_MbStructureType * Mbx, FlexCAN_MbType_e MbType);
static void FlexCAN_MbSetInterrupt(FlexCAN_Instance_e FlexCAN_Ins, FlexCAN_MbIndex_e MbIndex, bool IsEnableMbInt);
static void FlexCAN_SetModuleState(FlexCAN_Instance_e Ins, FlexCAN_State_e Transition);
static void FlexCAN_SetMBnumber(FLEXCAN_Type *FlexCANx, uint8_t MaxMB);

/* ----------------------------------------------------------------------------
   -- Private functions for interrupt handler
   ---------------------------------------------------------------------------- */
static void FlexCAN_Error_IRQHandler(FlexCAN_Instance_e Ins);
static void FlexCAN_BusOff_IRQHandler(FlexCAN_Instance_e Ins);
static void FlexCAN_MB_IRQHandler(FlexCAN_Instance_e Ins);

/* ----------------------------------------------------------------------------
   -- Handlers for FlexCAN interrupts
   ---------------------------------------------------------------------------- */
/* FlexCAN 0 interrupt handlers */
void CAN0_ORed_IRQHandler(void);
void CAN0_Error_IRQHandler(void);
void CAN0_ORed_0_15_MB_IRQHandler(void);
void CAN0_ORed_16_31_MB_IRQHandler(void);

/* FlexCAN 1 interrupt handlers */
void CAN1_Red_IRQHandler(void);
void CAN1_Error_IRQHandler(void);
void CAN1_ORed_0_15_MB_IRQHandler(void);
void CAN1_ORed_16_31_MB_IRQHandler(void);

/* FlexCAN 2 interrupt handlers */
void CAN2_ORed_IRQHandler(void);
void CAN2_Error_IRQHandler(void);
void CAN2_ORed_0_15_MB_IRQHandler(void);
void CAN2_ORed_16_31_MB_IRQHandler(void);

/* ----------------------------------------------------------------------------
   -- Global functions
   ---------------------------------------------------------------------------- */
FlexCAN_Driver_ReturnCode_e FlexCAN_Init(FlexCAN_Instance_e FlexCAN_Ins, FlexCAN_ConfigType *FlexCAN_Config)
{
    FlexCAN_Driver_ReturnCode_e RetVal = FLEXCAN_DRIVER_RETURN_CODE_ERROR;

    FLEXCAN_Type *FlexCANx = NULL;

   if(FlexCAN_Ins > FlexCAN2_INS || FlexCAN_Config == NULL || FlexCAN_CurrentState[FlexCAN_Ins] != FLEXCAN_STATE_UNINIT)
   {
       /* Invalid parameters or driver already in init state */
   }
   else
   {
        FlexCANx = FlexCAN_Base_Addr[FlexCAN_Ins];

        /* Disable the FlexCAN module */
        FlexCAN_ModuleControl(FlexCANx, DISABLE);

        /* Selects clock source for CAN module */
        FlexCAN_ClkSrcSelect(FlexCANx, FlexCAN_Config->CLkSrc);

        /* Enable FlexCAN module -> FlexCAN automatically enters Freeze Mode */
        FlexCAN_ModuleControl(FlexCANx, ENABLE);

        /* Sets desired bit rate for FlexCAN module */
        FlexCAN_SetBitRate(FlexCANx, FlexCAN_Config->ClkFreq, FlexCAN_Config->BitRate);

        /* Clear MSG Buffers */
        FlexCAN_ClearMB(FlexCANx);

        /* Select mode for FlexCAN module */
        FlexCAN_RunModeSelect(FlexCANx, FlexCAN_Config->RunMode);

        /* Interrupts controlling for FLexCAN module */
        FlexCAN_IntControl(FlexCANx, FlexCAN_Config->IntControl);

        FlexCAN_SetMBnumber(FlexCANx, FlexCAN_Config->MaxNoMB);

        /* Exit Freeze mode */
        FLexCAN_FreezeModeControl(FlexCANx, DISABLE);

        /* Switch to READY state */
        FlexCAN_SetModuleState(FlexCAN_Ins, FLEXCAN_STATE_READY);

        /* Inits successfully */
        RetVal = FLEXCAN_DRIVER_RETURN_CODE_SUCCESSED;
   }

   return RetVal;
}

FlexCAN_Driver_ReturnCode_e FlexCAN_DeInit(FlexCAN_Instance_e FlexCAN_Ins)
{
    FlexCAN_Driver_ReturnCode_e RetVal = FLEXCAN_DRIVER_RETURN_CODE_ERROR;

    FLEXCAN_Type *FlexCANx = NULL;

    if(FlexCAN_Ins > FlexCAN2_INS || FlexCAN_CurrentState[FlexCAN_Ins] != FLEXCAN_STATE_READY)
    {
        /* Invalid parameter or driver is not in ready  state */
    }
    else
    {
        FlexCANx = FlexCAN_Base_Addr[FlexCAN_Ins];

        /* Reset memory-mapped registers */
        FlexCAN_SoftReset(FlexCANx);

        /* Disable the FlexCAN module */
        FlexCAN_ModuleControl(FlexCANx, DISABLE);

        /* Switch to UNINIT state */
        FlexCAN_SetModuleState(FlexCAN_Ins, FLEXCAN_STATE_UNINIT);

        /* De-Inits successfully */
        RetVal = FLEXCAN_DRIVER_RETURN_CODE_SUCCESSED;
    }

    return RetVal;
}

FlexCAN_Driver_ReturnCode_e FlexCAN_CallbackRegister(FlexCAN_Instance_e Ins, FlexCAN_CallbackType CallbackFunc, uint8_t CallbackID)
{
    FlexCAN_Driver_ReturnCode_e RetVal = FLEXCAN_DRIVER_RETURN_CODE_ERROR;

    uint8_t CallbackIndex = CallbackID;

    if(Ins > FlexCAN2_INS || CallbackFunc == NULL)
    {
        /* Invalid parameters */
    }
    else
    {
        /* Saving handler function to the corresponding callback pointer */
        if(CallbackIndex < NUMBER_OF_MB)
        {
            FlexCAN_MbCallback[CallbackID] = CallbackFunc;
        }
        else
        {
            if(CallbackID == ERROR_CALLBACK_ID)
            {
                FlexCAN_Callback[Ins*ERROR_HANDLER_GAP] = CallbackFunc;
            }
            else
            {
                FlexCAN_Callback[Ins*ERROR_HANDLER_GAP + ORED_HANDLER_GAP] = CallbackFunc;
            }
        }

        RetVal = FLEXCAN_DRIVER_RETURN_CODE_SUCCESSED;
    }

    return RetVal;
}

FlexCAN_Driver_ReturnCode_e FlexCAN_MbInit(FlexCAN_Instance_e FlexCAN_Ins, FlexCAN_MbIndex_e MbIndex,
                    FlexCAN_MbHeaderType * FLexCAN_MbConfig)
{
    FlexCAN_Driver_ReturnCode_e RetVal = FLEXCAN_DRIVER_RETURN_CODE_ERROR;

    FlexCAN_MbStructureType * Mbx = NULL;

    if(FlexCAN_Ins > FlexCAN2_INS || MbIndex > MB31 || FLexCAN_MbConfig == NULL)
    {
        /* Invalid parameters */
    }
    else
    {
        Mbx = &((FlexCAN_MB[FlexCAN_Ins])->MB[MbIndex]);

        FlexCAN_MBSetEDL(Mbx, FLexCAN_MbConfig->EDL);

        FlexCAN_MBSetBRS(Mbx, FLexCAN_MbConfig->BRS);

        FlexCAN_MBSetESI(Mbx, FLexCAN_MbConfig->ESI);

        /* SRR must always be 1 */
        Mbx->Header[0] |= FLEXCAN_RAMn_DATA_WORD_0_SRR_MASK;

        FlexCAN_MBSetIDType(Mbx, FLexCAN_MbConfig->IdType);

        FlexCAN_MBSetRTR(Mbx, FLexCAN_MbConfig->IsRemote);

        FlexCAN_MBSetDataLength(Mbx, FLexCAN_MbConfig->DataLen);

        FlexCAN_MBSetID(Mbx, FLexCAN_MbConfig->MbID);

        FlexCAN_MbSetType(Mbx, FLexCAN_MbConfig->MbType);

        FlexCAN_MbSetInterrupt(FlexCAN_Ins, MbIndex, FLexCAN_MbConfig->IsEnableMbInt);

        RetVal = FLEXCAN_DRIVER_RETURN_CODE_SUCCESSED;
    }

    return RetVal;
}

FlexCAN_Driver_ReturnCode_e FlexCAN_Transmit(FlexCAN_Instance_e FlexCAN_Ins, FlexCAN_MbIndex_e MbIndex,
                      uint8_t * MsgData)
{
    FlexCAN_Driver_ReturnCode_e RetVal = FLEXCAN_DRIVER_RETURN_CODE_ERROR;

    FLEXCAN_Type * FlexCANx = NULL;
    FlexCAN_MbStructureType * Mbx = NULL;

    uint8_t DataLen = 0U;
    uint8_t Index = 0;
    uint8_t WordIndex = 0;
    uint8_t WordSize = 4;
    uint8_t ByteOffset = 0;

    if(FlexCAN_Ins > FlexCAN2_INS || MbIndex > MB31 || MsgData == NULL)
    {
        /* Invalid parameters*/
    }
    else
    {
        FlexCANx = FlexCAN_Base_Addr[FlexCAN_Ins];
        Mbx = &((FlexCAN_MB[FlexCAN_Ins])->MB[MbIndex]);
        DataLen = (((Mbx->Header[0]) & FLEXCAN_RAMn_DATA_WORD_0_DLC_MASK) >> FLEXCAN_RAMn_DATA_WORD_0_DLC_SHIFT);

        /* Clear Int Flag */
        FlexCANx->IFLAG1 &= ~(1 << MbIndex);

        /* Clear data */
        Mbx->Payload[0] = 0;
        Mbx->Payload[1] = 0;

        /* Changes state to STARTED */
        FlexCAN_SetModuleState(FlexCAN_Ins, FLEXCAN_STATE_STARTED);

        /* Message data to RAM */
        for(Index = 0; Index < DataLen; Index++)
        {
            WordIndex = Index / WordSize;
            ByteOffset = 3 - (Index % WordSize);
            Mbx->Payload[WordIndex] |= MsgData[Index] << (8 * ByteOffset);
        }

        /* Start transmitting */
        Mbx->Header[0] &= ~FLEXCAN_RAMn_DATA_WORD_0_CODE_MASK;
        Mbx->Header[0] |= FLEXCAN_RAMn_DATA_WORD_0_CODE(Tx_CODE_DATA);

        /* Changes state to READY */
        FlexCAN_SetModuleState(FlexCAN_Ins, FLEXCAN_STATE_READY);

        RetVal = FLEXCAN_DRIVER_RETURN_CODE_SUCCESSED;
    }

    return RetVal;
}

FlexCAN_Driver_ReturnCode_e FlexCAN_ReadMailboxData(FlexCAN_Instance_e FlexCAN_Ins, FlexCAN_MbIndex_e MbIndex,
                             uint8_t * MsgData)
{
    FlexCAN_Driver_ReturnCode_e RetVal = FLEXCAN_DRIVER_RETURN_CODE_SUCCESSED;

    FLEXCAN_Type * FlexCANx = NULL;
    FlexCAN_MbStructureType * Mbx = NULL;
    uint8_t DataLen = 0U;

    uint8_t Index = 0U;
    uint8_t WordIndex = 0U;
    uint8_t WordSize = 4U;
    uint8_t ByteOffset = 0U;
    
    if(FlexCAN_Ins > FlexCAN2_INS || MbIndex > MB31 || MsgData == NULL)
    {
        /* Invalid parameters */
    }
    else
    {
        FlexCANx = FlexCAN_Base_Addr[FlexCAN_Ins];
        Mbx = &((FlexCAN_MB[FlexCAN_Ins])->MB[MbIndex]);
        DataLen = (((Mbx->Header[0]) & FLEXCAN_RAMn_DATA_WORD_0_DLC_MASK)
                                >> FLEXCAN_RAMn_DATA_WORD_0_DLC_SHIFT);
    
        /* Changes state to STARTED */
        FlexCAN_SetModuleState(FlexCAN_Ins, FLEXCAN_STATE_STARTED);

        for(Index = 0; Index < DataLen; Index++)
        {
            WordIndex = (Index / WordSize);
            ByteOffset = 3 - (Index % WordSize);
            MsgData[Index] = ((Mbx->Payload[WordIndex]) >> (8 * ByteOffset));
        }

        /* Unlock the Mailbox */
        (void)FlexCANx->TIMER;

        /* Changes state to READY */
        FlexCAN_SetModuleState(FlexCAN_Ins, FLEXCAN_STATE_READY);

        RetVal = FLEXCAN_DRIVER_RETURN_CODE_SUCCESSED;
    }
    
    return RetVal;
}

FlexCAN_State_e FlexCAN_GetModuleState(FlexCAN_Instance_e Ins)
{
    FlexCAN_State_e RetState = FLEXCAN_STATE_UNINIT;

    RetState = FlexCAN_CurrentState[Ins];

    return RetState;
}

uint8_t FlexCAN_GetStatusFlag(FlexCAN_Instance_e Ins, FlexCAN_StatusFlag_e FlagType)
{
    FLEXCAN_Type *FlexCANx = FlexCAN_Base_Addr[Ins];

    /* Variable to store status flag value */
    uint8_t FlagValue = 0U;

    /* Get the value of the wanted status flag */
    if(FlagType == FlexCAN_STATUS_FLAG_FLTCONF)
    {
        FlagValue = (FlexCANx->ESR1 >> FlagType) & 0x3;
    }
    else
    {
        FlagValue = (FlexCANx->ESR1 >> FlagType) & SET;
    }

    return FlagValue;
}

/* ----------------------------------------------------------------------------
   -- Private functions
   ---------------------------------------------------------------------------- */
static void FlexCAN_ModuleControl(FLEXCAN_Type *FlexCANx, uint8_t EnOrDis)
{
    if(EnOrDis == ENABLE)
    {
        /* Enable the FlexCAN module */
       FlexCANx->MCR &= ~FLEXCAN_MCR_MDIS_MASK;

       /* Wait for FRZACK */
       while(((FlexCANx->MCR & FLEXCAN_MCR_FRZACK_MASK) >> FLEXCAN_MCR_FRZACK_SHIFT) != SET);
    }
    else
    {
        /* Disable the FlexCAN module */
       FlexCANx->MCR |= FLEXCAN_MCR_MDIS_MASK;
    }
}

static void FlexCAN_ClkSrcSelect(FLEXCAN_Type *FlexCANx, FlexCAN_ClkSrc_e CLkSrc)
{
    /* Selects clock source for FlexCAN module*/
   FlexCANx->CTRL1 &= ~FLEXCAN_CTRL1_CLKSRC_MASK;
   FlexCANx->CTRL1 |= FLEXCAN_CTRL1_CLKSRC(CLkSrc);
}

static void FlexCAN_ClearMB(FLEXCAN_Type *FlexCANx)
{
    uint8_t i = 0;

    /* Clears all MBs of FlexCAN module */
   for(i = 0U; i < 128U ;i++)
   {
       FlexCANx->RAMn[i] = 0x0;
   }
}

static void FlexCAN_RunModeSelect(FLEXCAN_Type *FlexCANx, FlexCAN_Mode_e RunMode)
{
   if(RunMode == FlexCAN_MODE_LOOPBACK)
   {
       /* Enables loop back mode */
       FlexCANx->CTRL1 |= FLEXCAN_CTRL1_LPB_MASK;
   }
   else
   {
       /* Enables normal mode */
       FlexCANx->CTRL1 &= ~FLEXCAN_CTRL1_LPB_MASK;
   }
}

static void FlexCAN_IntControl(FLEXCAN_Type *FlexCANx, FlexCAN_InterruptType IntType)
{
   /* Error interrupt configuration */
   if(IntType.IntError == FlexCAN_INT_ERROR_ENABLE)
   {
       /* Enable error interrupt */
       FlexCANx->CTRL1 |= FLEXCAN_CTRL1_ERRMSK_MASK;
   }
   else
   {
       /* Disable error interrupt */
       FlexCANx->CTRL1 &= ~FLEXCAN_CTRL1_ERRMSK_MASK;
   }

   /* ORed interrupt configuration */
   if((IntType.IntBusOff | IntType.IntTxWarning | IntType.IntRxWarning) == SET)
   {
       /* Bus Off Interrupt configuration */
       FlexCANx->CTRL1 &= ~FLEXCAN_CTRL1_BOFFMSK_MASK;
       FlexCANx->CTRL1 |= FLEXCAN_CTRL1_BOFFMSK(IntType.IntBusOff);

       /* Tx warning Interrupt configuration */
       FlexCANx->CTRL1 &= ~FLEXCAN_CTRL1_TWRNMSK_MASK;
       FlexCANx->CTRL1 |= FLEXCAN_CTRL1_TWRNMSK(IntType.IntTxWarning);

       /* Rx warning Interrupt configuration */
       FlexCANx->CTRL1 &= ~FLEXCAN_CTRL1_RWRNMSK_MASK;
       FlexCANx->CTRL1 |= FLEXCAN_CTRL1_RWRNMSK(IntType.IntRxWarning);
   }
   else
   {
       /* Interrupts disabled */
       FlexCANx->CTRL1 &= ~FLEXCAN_CTRL1_BOFFMSK_MASK;
       FlexCANx->CTRL1 &= ~FLEXCAN_CTRL1_BOFFMSK_MASK;
       FlexCANx->CTRL1 &= ~FLEXCAN_CTRL1_BOFFMSK_MASK;
   }
}

static void FlexCAN_SetBitRate(FLEXCAN_Type *FlexCANx, uint32_t Clocks, uint32_t BitRate)
{
    uint32_t prescaler = 0;
    uint32_t prop_seg = 0;
    uint32_t tseg = 0;
    uint32_t actual_baud = 0;
    uint32_t best_prescaler = 0, best_prop_seg = 0, best_tseg = 0;
    uint32_t min_error = UINT32_MAX;

    for (prop_seg = 1; prop_seg <= 7; prop_seg++)
    {
        for (tseg = 2; tseg <= 8; tseg++)
        {
            prescaler = (Clocks / (BitRate * (1 + prop_seg + 2 * tseg))) - 1;

            if (prescaler > 255 || prescaler < 0)
                continue;
            if ((1 + prop_seg + 2 * tseg) < 11 || (1 + prop_seg + 2 * tseg) > 21)
                continue;
            if (((tseg * 100) / (1 + prop_seg + 2 * tseg) > 25) || ((tseg * 100) / (1 + prop_seg + 2 * tseg) < 20))

                actual_baud = Clocks / ((prescaler + 1) * (1 + prop_seg + 2 * tseg));

            uint32_t error = (actual_baud > BitRate) ? (actual_baud - BitRate) : (BitRate - actual_baud);

            if (error < min_error)
            {
                min_error = error;
                best_prescaler = prescaler;
                best_prop_seg = prop_seg - 1;
                best_tseg = tseg - 1;
            }
        }
    }
    FlexCANx->CTRL1 |= (best_prescaler << FLEXCAN_CTRL1_PRESDIV_SHIFT) | (best_prop_seg << FLEXCAN_CTRL1_PROPSEG_SHIFT) | (best_tseg << FLEXCAN_CTRL1_PSEG1_SHIFT) | (best_tseg << FLEXCAN_CTRL1_PSEG2_SHIFT);
}

static void FLexCAN_FreezeModeControl(FLEXCAN_Type *FlexCANx, uint8_t EnOrDis)
{
    if(EnOrDis == ENABLE)
    {
        /* Enters freeze mode */
        FlexCANx->MCR |= FLEXCAN_MCR_FRZ_MASK;

       /* Wait for FRZACK */
       while(((FlexCANx->MCR & FLEXCAN_MCR_FRZACK_MASK) >> FLEXCAN_MCR_FRZACK_SHIFT) != SET);
    }
    else
    {
        /* Exits freeze mode */
        FlexCANx->MCR &= ~FLEXCAN_MCR_FRZ_MASK;

       /* Wait for FRZACK to clear (not in freeze mode) */
       while ((FlexCANx->MCR & FLEXCAN_MCR_FRZACK_MASK) >> FLEXCAN_MCR_FRZACK_SHIFT);

       /* Wait for NOTRDY to clear (module ready) */
       while ((FlexCANx->MCR & FLEXCAN_MCR_NOTRDY_MASK) >> FLEXCAN_MCR_NOTRDY_SHIFT);
    }
}

static void FlexCAN_SoftReset(FLEXCAN_Type *FlexCANx)
{
    /* Reset memory-mapped registers */
    FlexCANx->MCR |= FLEXCAN_MCR_SOFTRST_MASK;

    /* Wait for the reset procedure completed */
    while(((FlexCANx->MCR & FLEXCAN_MCR_SOFTRST_MASK) >> FLEXCAN_MCR_SOFTRST_SHIFT) == SET);
}

static void FlexCAN_MBSetEDL(FlexCAN_MbStructureType * Mbx, uint8_t EDLValue)
{
    Mbx->Header[0] &= ~FLEXCAN_RAMn_DATA_WORD_0_EDL_MASK;
    Mbx->Header[0] |= FLEXCAN_RAMn_DATA_WORD_0_EDL(EDLValue);
}

static void FlexCAN_MBSetBRS(FlexCAN_MbStructureType * Mbx, uint8_t BRSValue)
{
    Mbx->Header[0] &= ~FLEXCAN_RAMn_DATA_WORD_0_BRS_MASK;
    Mbx->Header[0] |= FLEXCAN_RAMn_DATA_WORD_0_BRS(BRSValue);
}

static void FlexCAN_MBSetESI(FlexCAN_MbStructureType * Mbx, uint8_t ESIValue)
{
    Mbx->Header[0] &= ~FLEXCAN_RAMn_DATA_WORD_0_ESI_MASK;
    Mbx->Header[0] |= FLEXCAN_RAMn_DATA_WORD_0_ESI(ESIValue);
}

static void FlexCAN_MBSetIDType(FlexCAN_MbStructureType * Mbx, FlexCAN_MsgIDType_e IDType)
{
    Mbx->Header[0] &= ~FLEXCAN_RAMn_DATA_WORD_0_IDE_MASK;
    Mbx->Header[0] |= FLEXCAN_RAMn_DATA_WORD_0_IDE(IDType);
}

static void FlexCAN_MBSetRTR(FlexCAN_MbStructureType * Mbx, bool IsRemote)
{
    Mbx->Header[0] &= ~FLEXCAN_RAMn_DATA_WORD_0_RTR_MASK;
    Mbx->Header[0] |= FLEXCAN_RAMn_DATA_WORD_0_RTR(IsRemote);
}

static void FlexCAN_MBSetDataLength(FlexCAN_MbStructureType * Mbx, uint16_t Datalen)
{
    Mbx->Header[0] &= ~FLEXCAN_RAMn_DATA_WORD_0_DLC_MASK;
    Mbx->Header[0] |= FLEXCAN_RAMn_DATA_WORD_0_DLC(Datalen);
}

static void FlexCAN_MBSetID(FlexCAN_MbStructureType * Mbx, uint16_t ID)
{
    Mbx->Header[1] &= ~FLEXCAN_RAMn_DATA_WORD_1_ID_MASK;
    Mbx->Header[1] |= FLEXCAN_RAMn_DATA_WORD_1_ID(ID);
}

static void FlexCAN_MbSetType(FlexCAN_MbStructureType * Mbx, FlexCAN_MbType_e MbType)
{
    Mbx->Header[0] &= ~FLEXCAN_RAMn_DATA_WORD_0_CODE_MASK;

    if(MbType == FlexCAN_MB_TX)
    {
        /* Set code to INACTIVE status */
        Mbx->Header[0] |= FLEXCAN_RAMn_DATA_WORD_0_CODE(Tx_CODE_INACTIVE);
    }else
    {
        Mbx->Header[0] |= FLEXCAN_RAMn_DATA_WORD_0_CODE(Rx_CODE_EMPTY);
    }
}

static void FlexCAN_MbSetInterrupt(FlexCAN_Instance_e FlexCAN_Ins, FlexCAN_MbIndex_e MbIndex, bool IsEnableMbInt){
    FLEXCAN_Type * FlexCANx = FlexCAN_Base_Addr[FlexCAN_Ins];

    if(IsEnableMbInt)
    {
        FlexCANx->IMASK1 |= (1 << MbIndex);
    }
    else
    {
        FlexCANx->IMASK1 &= ~(1 << MbIndex);
    }
}

static void FlexCAN_SetModuleState(FlexCAN_Instance_e Ins, FlexCAN_State_e Transition)
{
    if(FlexCAN_CurrentState[Ins] != Transition)
    {
        /* Switching for desired state */
        FlexCAN_CurrentState[Ins] = Transition;
    }
    else
    {
        /* Current state is already in desired state */
    }
}

static void FlexCAN_SetMBnumber(FLEXCAN_Type *FlexCANx, uint8_t MaxMB)
{
    uint8_t NoMB = 0U;

    if(MaxMB >= NUMBER_OF_MB)
    {
        NoMB = NUMBER_OF_MB - 1U;
    }
    else
    {
        NoMB = MaxMB;
    }
    FlexCANx->MCR &= ~FLEXCAN_MCR_MAXMB_MASK;
    FlexCANx->MCR |= FLEXCAN_MCR_MAXMB(NoMB);
}

/* ----------------------------------------------------------------------------
   -- Private functions for interrupt handler
   ---------------------------------------------------------------------------- */
static void FlexCAN_Error_IRQHandler(FlexCAN_Instance_e Ins)
{
    FLEXCAN_Type *FlexCANx = FlexCAN_Base_Addr[Ins];

    /* Invoke callback */
    if(FlexCAN_Callback[Ins*ERROR_HANDLER_GAP] != NULL)
    {
        FlexCAN_Callback[Ins*ERROR_HANDLER_GAP]();
    }
    else
    {
        /* Callback is not registered */
    }

    /* Clear all other error interrupts in ESR1 register */
    FlexCANx->ESR1 |= ERROR_INT;
}

static void FlexCAN_BusOff_IRQHandler(FlexCAN_Instance_e Ins)
{
    FLEXCAN_Type *FlexCANx = FlexCAN_Base_Addr[Ins];

    /* Invoke callback */
    if(FlexCAN_Callback[Ins*ERROR_HANDLER_GAP + ORED_HANDLER_GAP] != NULL)
    {
        FlexCAN_Callback[Ins*ERROR_HANDLER_GAP + ORED_HANDLER_GAP]();
    }
    else
    {
        /* Callback is not registered */
    }

    /* Clear all BusOff and Tx/Rx Warning interrupts in ESR1 register */
    FlexCANx->ESR1 |= BUS_OFF_INT;
}

static void FlexCAN_MB_IRQHandler(FlexCAN_Instance_e Ins)
{
    FLEXCAN_Type *FlexCANx = FlexCAN_Base_Addr[Ins];

    /* Variable for raised MB interrupt flag */
    uint8_t RaisedFlag = 0;

    /* Variable for MBx's index */
    uint8_t MbIndex = 0;

    /* Get the interrupts that are enabled and ready */
    while((RaisedFlag & SET) == 0U && (MbIndex < NUMBER_OF_MB))
    {
        /* Check if IFLAG is set with the corresponding IMASK bit */
        RaisedFlag = (((FlexCANx->IFLAG1 & (FlexCANx->IMASK1 & FLEXCAN_IMASK1_BUF31TO0M_MASK)) >> MbIndex) & SET);

        /* Forward to check next MB interrupt */
        MbIndex++;
    }

    MbIndex--;

    if(RaisedFlag == SET)
    {
        /* Invoke callback */
        if(FlexCAN_MbCallback[MbIndex] != NULL)
        {
            FlexCAN_MbCallback[MbIndex]();
        }
        else
        {
            /* Callback is not registered */
        }

        /* Clear the corresponding IFLAG */
        FlexCANx->IFLAG1 |= (SET << MbIndex);
    }
    else
    {
        /* No interrupt occurs */
    }
}

/* ----------------------------------------------------------------------------
   -- Handlers for FlexCAN interrupts
   ---------------------------------------------------------------------------- */
/*
 * FlexCAN 0 Interrupt Handlers
 * - These handlers manage interrupts for FlexCAN module 0.
 * - Each interrupt is handled by a specific function in the FlexCAN driver,
 *   with FlexCAN0_INS passed to specify the CAN instance.
 */
void CAN0_ORed_IRQHandler()
{
    /* Handles bus-off condition for FlexCAN0, triggered when the CAN bus is off, TX warning or RX warning. */
    FlexCAN_BusOff_IRQHandler(FlexCAN0_INS);
}

void CAN0_Error_IRQHandler()
{
    /* Handles error conditions for FlexCAN0, triggered on error frames or bit errors. */
    FlexCAN_Error_IRQHandler(FlexCAN0_INS);
}

void CAN0_ORed_0_15_MB_IRQHandler()
{
    /* Handles message buffer interrupts for FlexCAN0, message buffers 0-15. */
    FlexCAN_MB_IRQHandler(FlexCAN0_INS);
}

void CAN0_ORed_16_31_MB_IRQHandler()
{
    /* Handles message buffer interrupts for FlexCAN0, message buffers 16-31. */
    FlexCAN_MB_IRQHandler(FlexCAN0_INS);
}

/*
 * FlexCAN 1 Interrupt Handlers
 * - These handlers manage interrupts for FlexCAN module 1.
 * - FlexCAN1_INS is passed to specify the CAN instance for FlexCAN1-specific processing.
 */
void CAN1_ORed_IRQHandler()
{
    /* Handles bus-off, TX warning or RX warning condition for FlexCAN1. */
    FlexCAN_BusOff_IRQHandler(FlexCAN1_INS);
}

void CAN1_Error_IRQHandler()
{
    /* Handles error conditions for FlexCAN1. */
    FlexCAN_Error_IRQHandler(FlexCAN1_INS);
}

void CAN1_ORed_0_15_MB_IRQHandler()
{
    /* Handles message buffer interrupts for FlexCAN1, message buffers 0-15. */
    FlexCAN_MB_IRQHandler(FlexCAN1_INS);
}

void CAN1_ORed_16_31_MB_IRQHandler()
{
    /* Handles message buffer interrupts for FlexCAN1, message buffers 16-31. */
    FlexCAN_MB_IRQHandler(FlexCAN1_INS);
}

/*
 * FlexCAN 2 Interrupt Handlers
 * - These handlers manage interrupts for FlexCAN module 2.
 * - FlexCAN2_INS is used to specify FlexCAN2-specific processing in each handler.
 */
void CAN2_ORed_IRQHandler()
{
    /* Handles bus-off, TX warning or RX warning condition for FlexCAN2. */
    FlexCAN_BusOff_IRQHandler(FlexCAN2_INS);
}

void CAN2_Error_IRQHandler()
{
    /* Handles error conditions for FlexCAN2. */
    FlexCAN_Error_IRQHandler(FlexCAN2_INS);
}

void CAN2_ORed_0_15_MB_IRQHandler()
{
    /* Handles message buffer interrupts for FlexCAN2, message buffers 0-15. */
    FlexCAN_MB_IRQHandler(FlexCAN2_INS);
}

void CAN2_ORed_16_31_MB_IRQHandler()
{
    /* Handles message buffer interrupts for FlexCAN2, message buffers 16-31. */
    FlexCAN_MB_IRQHandler(FlexCAN2_INS);
}

/* ----------------------------------------------------------------------------
   -- End of file
   ---------------------------------------------------------------------------- */

