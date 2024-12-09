/*
 * MIDDLE_FlexCAN.c
 *
 *  Created on: Nov 4, 2024
 *      Author: adm
 */

#include "MIDDLE_FLexCAN.h"
#include "CLOCK.h"
#include "PORT_Driver.h"

/* ----------------------------------------------------------------------------
   -- Definitions
   ---------------------------------------------------------------------------- */

typedef enum
{
	CAN_MB_INACTIVE 	= 0U,
	CAN_MB_ACTIVE		= 1U
}CAN_MbStatus_e;

/**
 * Array of PORT for instances FLEXCAN modules. Indexed by FLEXCAN instance number.
 */
#define FLEXCAN_PORT_INSTANCE { PORTE_INSTANCE, PORTC_INSTANCE, PORTB_INSTANCE }

/**
 * PCC (Peripheral Clock Controller) indices for each FLEXCAN instance.
 */
#define PCC_FLEXCAN_Index { PCC_FlexCAN0_INDEX, PCC_FlexCAN1_INDEX, PCC_FlexCAN2_INDEX }

/**
 * PCC (Peripheral Clock Controller) indices for each FLEXCAN instance.
 */
#define FLEXCAN_MUX_Index { portMuxAlt5, portMuxAlt3, portMuxAlt4 }

#define FLEXCAN_MB_BASE_PTR { MB_FLEXCAN_0, MB_FLEXCAN_1, MB_FLEXCAN_2 }

/**
 * NVIC (Nested Vector Interrupt Controller) indices for LPUART receive and transmit interrupts.
 * Each entry corresponds to an LPUART instance's Rx/Tx interrupt number.
 */
#define NVIC_FLEXCAN_Index { CAN0_ORed_IRQn, CAN0_Error_IRQn, CAN1_ORed_IRQn, CAN1_Error_IRQn, CAN2_ORed_IRQn, CAN2_Error_IRQn }

#define NVIC_MB_FLEXCAN_INDEX {CAN0_ORed_0_15_MB_IRQn, CAN1_ORed_0_15_MB_IRQn, CAN2_ORed_0_15_MB_IRQn, CAN0_ORed_16_31_MB_IRQn};

#define NVIC_FLEXCAN_COUNT	6U

#define NVIC_FLEXCAN_MB_COUNT	4U

#define FLEXCAN_GET_FREQ(ClkSrc) \
		((ClkSrc == FlexCAN_CLKSRC_SYS) ? SCG_GetSysFreq() :\
		 (ClkSrc == FlexCAN_CLKSRC_OSC) ? 40000000U 	   : 0)

/* ----------------------------------------------------------------------------
   -- Variables
   ---------------------------------------------------------------------------- */
/**
 * PCC (Peripheral Clock Controller) indices for FLEXCAN instances.
 * @note This array is indexed by LPUART instance numbers.
 */
static uint8_t PCC_FlexCAN[FLEXCAN_INSTANCE_COUNT] = PCC_FLEXCAN_Index;

static uint8_t FlexCAN_PORT[FLEXCAN_INSTANCE_COUNT] = FLEXCAN_PORT_INSTANCE;

static uint8_t FlexCAN_MUX[FLEXCAN_INSTANCE_COUNT] = FLEXCAN_MUX_Index;

/**
 * Array to map FLEXCAN instances to their corresponding NVIC IRQ numbers.
 *
 * This array holds the NVIC IRQ numbers corresponding to each FLEXCAN instance.
 * @note This array is indexed by FLEXCAN instance numbers.
 */
static IRQn_Type NVIC_FLEXCAN[NVIC_FLEXCAN_COUNT]	= NVIC_FLEXCAN_Index;

static IRQn_Type NVIC_MBFLEXCAN[NVIC_FLEXCAN_MB_COUNT]	= NVIC_MB_FLEXCAN_INDEX;

CAN_MbStatus_e AllMbStatus[32] = { CAN_MB_INACTIVE };

/* ----------------------------------------------------------------------------
   -- Private functions
   ---------------------------------------------------------------------------- */
static void FlexCAN_PORT_Init(MID_CAN_ModuleIns_e Ins, FlexCAN_PinType PortPin);
static void FlexCAN_NVIC_Control(MID_CAN_ModuleIns_e Ins, FlexCAN_InterruptType IntControl);
static void FlexCAN_NVIC_MbControl(MID_CAN_ModuleIns_e Ins, FlexCAN_MbIndex_e MbIndex, bool IsEnableInt);

/* ----------------------------------------------------------------------------
   -- Global functions
   ---------------------------------------------------------------------------- */
void MID_CAN_Init(MID_CAN_ModuleIns_e Ins)
{
	FlexCAN_ConfigType FlexCANConfig;

	/* Configuration elements for FlexCAN module */
	FlexCANConfig.BitRate = 500000;
	FlexCANConfig.MaxNoMB = 31U;
	FlexCANConfig.CLkSrc = FlexCAN_CLKSRC_SYS;
	FlexCANConfig.IntControl.IntError = FlexCAN_INT_ERROR_ENABLE;
	FlexCANConfig.PortPin.TxPin = FlexCAN0_Tx;
	FlexCANConfig.PortPin.RxPin = FlexCAN0_Rx;
	FlexCANConfig.RunMode = FlexCAN_MODE_NORMAL;
	FlexCANConfig.ClkFreq = FLEXCAN_GET_FREQ(FlexCANConfig.CLkSrc);

	/* PORT initialization */
	FlexCAN_PORT_Init(Ins, FlexCANConfig.PortPin);

	/* Enable clock for FlexCANx */
	PCC_PeriClockControl(PCC_FlexCAN[Ins], CLOCK_NOSRC_CLK, CLOCK_DIV_DISABLED, ENABLE);

	/* NVIC IRQ line configuration for FlexCAN interrupts */
	FlexCAN_NVIC_Control(Ins, FlexCANConfig.IntControl);

	/* Initializes FlexCAN driver */
	FlexCAN_Init(Ins, &FlexCANConfig);
}

void MID_CAN_DeInit(MID_CAN_ModuleIns_e Ins)
{
	/* Deinit FlexCAN Driver */
	FlexCAN_DeInit(Ins);

	/* Disable FlexCAN clock */
	PCC_PeriClockControl(PCC_FlexCAN[Ins], CLOCK_NOSRC_CLK, CLOCK_DIV_DISABLED, DISABLE);

	/* Disable FlexCAN PORT clock */
	PCC_PeriClockControl(FlexCAN_PORT[Ins], CLOCK_NOSRC_CLK, CLOCK_DIV_DISABLED, DISABLE);
}

void MID_CAN_SetCallback(MID_CAN_ModuleIns_e Ins, MID_CAN_UserConfigType *UserConfig)
{
	uint8_t CallbackID = 0U;

	CallbackID = UserConfig->HandlerType;

	FlexCAN_CallbackRegister(Ins, UserConfig->HandlerFunc, CallbackID);
}

void MID_CAN_StdRxMbInit(MID_CAN_ModuleIns_e Ins, MID_CAN_UserConfigType *UserConfig)
{
	FlexCAN_Driver_ReturnCode_e InitCode = FLEXCAN_DRIVER_RETURN_CODE_ERROR;

	FlexCAN_MbHeaderType MB = {
			.EDL = 0,
			.BRS = 0,
			.ESI = 0,
			.MbType = FlexCAN_MB_RX,
			.IdType = FlexCAN_STANDARD,
			.IsRemote = false,
			.DataLen = 1,
			.MbID = UserConfig->MbID,
			.IsEnableMbInt = UserConfig->MbInt
	};

	InitCode = FlexCAN_MbInit(Ins, UserConfig->MbIndex, &MB);

	/* Checks if initialization is successed or not */
	if(InitCode == FLEXCAN_DRIVER_RETURN_CODE_SUCCESSED)
	{
		AllMbStatus[UserConfig->MbIndex] = CAN_MB_ACTIVE;
		FlexCAN_NVIC_MbControl(Ins, UserConfig->MbIndex, UserConfig->MbInt);
	}
	else
	{
		AllMbStatus[UserConfig->MbIndex] = CAN_MB_INACTIVE;
	}
}

void MID_CAN_StdTxMbInit(MID_CAN_ModuleIns_e Ins, MID_CAN_UserConfigType *UserConfig)
{
	FlexCAN_Driver_ReturnCode_e InitCode = FLEXCAN_DRIVER_RETURN_CODE_ERROR;

	FlexCAN_MbHeaderType MB = {
			.EDL = 0,
			.BRS = 0,
			.ESI = 0,
			.MbType = FlexCAN_MB_TX,
			.IdType = FlexCAN_STANDARD,
			.IsRemote = false,
			.DataLen = 1,
			.MbID = UserConfig->MbID,
			.IsEnableMbInt = UserConfig->MbInt
	};

	InitCode = FlexCAN_MbInit(Ins, UserConfig->MbIndex, &MB);

	/* Checks if initialization is successed or not */
	if(InitCode == FLEXCAN_DRIVER_RETURN_CODE_SUCCESSED)
	{
		AllMbStatus[UserConfig->MbIndex] = CAN_MB_ACTIVE;
		FlexCAN_NVIC_MbControl(Ins, UserConfig->MbIndex, UserConfig->MbInt);
	}
	else
	{
		AllMbStatus[UserConfig->MbIndex] = CAN_MB_INACTIVE;
	}
}

void MID_CAN_Transmit(MID_CAN_ModuleIns_e Ins, FlexCAN_MbIndex_e MbIndex, uint8_t *TxBuffer)
{
	CAN_MbStatus_e MbStatus = CAN_MB_INACTIVE;

	MbStatus = AllMbStatus[MbIndex];

	if(MbStatus == CAN_MB_ACTIVE)
	{
		FlexCAN_Transmit(Ins, MbIndex, TxBuffer);
	}
	else
	{
		/* Mb is inactive */
	}
}

void MID_CAN_Receive(MID_CAN_ModuleIns_e Ins, FlexCAN_MbIndex_e MbIndex, uint8_t *RxBuffer)
{
	CAN_MbStatus_e MbStatus = CAN_MB_INACTIVE;

	MbStatus = AllMbStatus[MbIndex];

	if(MbStatus == CAN_MB_ACTIVE)
	{
		FlexCAN_ReadMailboxData(Ins, MbIndex, RxBuffer);
	}
	else
	{
		/* Mb is inactive */
	}
}

uint8_t MID_CAN_GetAckStatus(MID_CAN_ModuleIns_e Ins)
{
	uint8_t FlagValue = 0U;

	FlagValue = FlexCAN_GetStatusFlag(Ins, FlexCAN_STATUS_FLAG_ACKERR);

	return FlagValue;
}

/* ----------------------------------------------------------------------------
   -- Private functions
   ---------------------------------------------------------------------------- */

static void FlexCAN_PORT_Init(MID_CAN_ModuleIns_e Ins, FlexCAN_PinType PortPin)
{
	PORT_Config_type	   PORTConfig;
	PORT_PinConfig_type PORTPINConfig;


	/* PORT Initialization for FlexCANx */
	PORTConfig.muxMode = FlexCAN_MUX[Ins];
	PCC_PeriClockControl(PCC_PORTE_INDEX, CLOCK_NOSRC_CLK, CLOCK_DIV_DISABLED, ENABLE);
	PORTPINConfig.userConfig = PORTConfig;
	PORTPINConfig.pinCode = FlexCAN_PORT[Ins]*32 + PortPin.TxPin;
	PORT_Driver_InitPin(&PORTPINConfig);
	PORTPINConfig.pinCode = FlexCAN_PORT[Ins]*32 + PortPin.RxPin;
	PORT_Driver_InitPin(&PORTPINConfig);
}

static void FlexCAN_NVIC_Control(MID_CAN_ModuleIns_e Ins, FlexCAN_InterruptType IntControl)
{
	/* Error Interrupt NVIC configuration */
	if(IntControl.IntError == FlexCAN_INT_ERROR_ENABLE)
	{
	   /* Enable corresponding NVIC IRQ line */
	   NVIC_EnableIRQn(NVIC_FLEXCAN[(Ins*2U) + 1U]);
	}
	else
	{
	   /* Disable corresponding NVIC IRQ line */
	   NVIC_DisableIRQn(NVIC_FLEXCAN[(Ins*2U) + 1U]);
	}

	/* ORed Interrupt NVIC configuration */
	if((IntControl.IntBusOff | IntControl.IntRxWarning | IntControl.IntTxWarning) == ENABLE)
	{
	   /* Enable corresponding NVIC IRQ line */
	   NVIC_EnableIRQn(NVIC_FLEXCAN[Ins*2U]);
	}
	else
	{
	   /* Enable corresponding NVIC IRQ line */
	   NVIC_DisableIRQn(NVIC_FLEXCAN[Ins*2U]);
	}
}

static void FlexCAN_NVIC_MbControl(MID_CAN_ModuleIns_e Ins, FlexCAN_MbIndex_e MbIndex, bool IsEnableInt){
	IRQn_Type IRQNumber = NVIC_MBFLEXCAN[0];

	if(Ins == MODULE_0_INS)
	{
		if(MbIndex < 16)
		{
			IRQNumber = NVIC_MBFLEXCAN[0];
		}
		else
		{
			IRQNumber = NVIC_MBFLEXCAN[3];
		}
	}
	else
	{
		IRQNumber = NVIC_MBFLEXCAN[Ins];
	}

	if(IsEnableInt == true)
	{
		NVIC_EnableIRQn(IRQNumber);
	}
	else
	{
		NVIC_DisableIRQn(IRQNumber);
	}
}
