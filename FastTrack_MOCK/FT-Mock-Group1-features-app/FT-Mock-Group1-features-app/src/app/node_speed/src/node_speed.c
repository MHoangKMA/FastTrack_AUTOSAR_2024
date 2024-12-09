/*
 * node_speed.c
 *
 *  Created on: Nov 18, 2024
 *      Author: Manh
 */

/******************************************************************************/
/* Includes */
/******************************************************************************/

#include "node_speed.h"

/******************************************************************************/
/* Variables */
/******************************************************************************/

uint8_t MsgDataSpeed[2] = "OK";								/* Message data for speed ping */
uint8_t value = 0;											/* Variable to store ADC data */
Speed_Ping_State_t Speed_Ping_State = SPEED_PING_NOT_READY; /* Ping state */
Speed_Connect_State_t Speed_Connect_State = SPEED_OK;		/* Connect state */

/******************************************************************************/
/* Static APIs */
/******************************************************************************/

/**
 * @brief Reads ADC data and transmits it via CAN.
 */
static void App_Read_Send_Speed_Data(void)
{
	value = MID_ADC_ReadData();
	MID_CAN_Transmit(MODULE_0_INS, MB0, &value);
}

/**
 * @brief Processes the speed ping state and transmits a ping message if ready.
 */
static void App_ProcessSpeedPing(Speed_Ping_State_t *State)
{
	if (*State == SPEED_PING_READY)
	{
		MID_CAN_Transmit(MODULE_0_INS, MB1, MsgDataSpeed);
		*State = SPEED_PING_NOT_READY;
	}
}

/**
 * @brief Checks the connection status of the speed module.
 */
static void App_CheckSpeedConnect()
{
	uint8_t ACK_State = 0;
	ACK_State = MID_CAN_GetAckStatus(MODULE_0_INS);
	if (ACK_State == 1)
	{
		Speed_Connect_State = SPEED_NOT_OK;
	}
}

/**
 * @brief Reconnects the speed module if it is disconnected.
 */
static void App_SpeedReconnect(void)
{
	uint8_t ACK_State = 0;
	ACK_State = MID_CAN_GetAckStatus(MODULE_0_INS);
	if (Speed_Connect_State == SPEED_NOT_OK && ACK_State == 0)
	{
		App_Read_Send_Speed_Data();
		Speed_Connect_State = SPEED_OK;
	}
}

/******************************************************************************/
/* Callback APIs */
/******************************************************************************/

/**
 * @brief Callback for handling ADC data reception.
 */
void App_Speed_ADC_Notification(uint16_t x)
{
	MID_CAN_Transmit(MODULE_0_INS, MB0, (uint8_t *)&x);
}

/**
 * @brief Callback for LPIT timer events to update speed ping state.
 */
void LPIT_Callback_Speed(uint8_t channel)
{
	switch (channel)
	{
	case 0:
		Speed_Ping_State = SPEED_PING_READY;
		break;
	default:
		break;
	}
}

/**
 * @brief Processes incoming requests by reading and sending speed data.
 */
void App_Speed_RcvRequest(void)
{
	App_Read_Send_Speed_Data();
}

/******************************************************************************/
/* Public APIs */
/******************************************************************************/

/**
 * @brief Main function to initialize and manage speed node operations.
 */
void App_NodeSpeed_Run()
{
	/* Configuration structure for the speed node */
	Node_Config_Data_Struct_type Node_Speed_Cfg =
		{
			.nodeType = NODE_TYPE_SPEEED,
			.threshold = 1};

	/* ADC configuration structure for the speed node */
	MID_ADC_ConfigStruct_type ADC_Cfg_Speed =
		{
			.adcHwUnitId = IP_ADC0,
			.channel = ADC_CHANNEL_12,
			.nodeConfigPtr = &Node_Speed_Cfg,
			.callback = App_Speed_ADC_Notification};
	MID_ADC_Init(&ADC_Cfg_Speed);
	MID_LPIT_Init(LPIT_INS_0, LPIT_Callback_Speed);
	MID_CAN_Init(MODULE_0_INS);

	/* Configuration for sending data message */
	MID_CAN_UserConfigType UserCfgMBSendData = {
		.HandlerFunc = NULL,
		.MbID = 0x22,
		.MbIndex = MB0,
		.MbInt = true};

	/* Configuration for sending ping message */
	MID_CAN_UserConfigType UserCfgMBSendPing = {
		.HandlerFunc = NULL,
		.MbID = 0x44,
		.MbIndex = MB1,
		.MbInt = true};

	MID_CAN_StdTxMbInit(MODULE_0_INS, &UserCfgMBSendData);
	MID_CAN_StdTxMbInit(MODULE_0_INS, &UserCfgMBSendPing);

	/* Configuration for handling incoming requests */
	MID_CAN_UserConfigType UserCfgMBRequest = {
		.HandlerFunc = App_Speed_RcvRequest,
		.HandlerType = MIDDLE_HANDLER_MB_2_TYPE,
		.MbID = 0x55,
		.MbIndex = MB2,
		.MbInt = true};

	MID_CAN_StdRxMbInit(MODULE_0_INS, &UserCfgMBRequest);
	MID_CAN_SetCallback(MODULE_0_INS, &UserCfgMBRequest);

	MID_LPIT_StartTimer(LPIT_INS_0, 0, 6000000);

	App_Read_Send_Speed_Data();
	while (1)
	{
		App_ProcessSpeedPing(&Speed_Ping_State);
		App_CheckSpeedConnect();
		App_SpeedReconnect();
	}
}
