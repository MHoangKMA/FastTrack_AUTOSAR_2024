/*
 * node_temp.c
 *
 *  Created on: Nov 18, 2024
 *      Author: Manh
 */

/******************************************************************************/
/* Includes */
/******************************************************************************/

#include "node_temp.h"

/******************************************************************************/
/* Variables */
/******************************************************************************/

uint8_t MsgDataTemp[2] = "OK";
Temp_Ping_State_t Temp_Ping_State = TEMP_PING_NOT_READY;
Temp_Connect_State_t Temp_Connect_State = TEMP_OK;
uint8_t Temp_value = 0;

/******************************************************************************/
/* Local APIs */
/******************************************************************************/

/**
 * @brief Reads temperature data via ADC and transmits it via CAN.
 */
static void App_Read_Send_Temp_Data(void)
{
	Temp_value = MID_ADC_ReadData();
	MID_CAN_Transmit(FlexCAN0_INS, MB0, &Temp_value);
}

/**
 * @brief Processes the temperature ping state and sends a ping message if ready.
 */
static void App_ProcessTempPing(Temp_Ping_State_t *State)
{
	if (*State == TEMP_PING_READY)
	{
		FlexCAN_Transmit(FlexCAN0_INS, MB1, MsgDataTemp);
		*State = TEMP_PING_NOT_READY;
	}
}

/**
 * @brief Checks the connection status of the temperature module.
 */
static void App_CheckTempConnect(void)
{
	uint8_t ACK_State = 0;
	ACK_State = MID_CAN_GetAckStatus(MODULE_0_INS);
	if (ACK_State == 1)
	{
		Temp_Connect_State = TEMP_NOT_OK;
	}
}

/**
 * @brief Reconnects the temperature module if it is disconnected.
 */
static void App_TempReconnect(void)
{
	uint8_t ACK_State = 0;
	ACK_State = MID_CAN_GetAckStatus(MODULE_0_INS);
	if (Temp_Connect_State == TEMP_NOT_OK && ACK_State == 0)
	{
		App_Read_Send_Temp_Data();
		Temp_Connect_State = TEMP_OK;
	}
}

/******************************************************************************/
/* CallBack APIs */
/******************************************************************************/

/**
 * @brief Callback for handling ADC temperature data reception.
 */
void App_Temp_ADC_Notification(uint16_t x)
{
	FlexCAN_Transmit(FlexCAN0_INS, MB0, (uint8_t *)&x);
}

/**
 * @brief Callback for LPIT timer events to update the temperature ping state.
 */
void LPIT_Callback_Temp(uint8_t channel)
{
	switch (channel)
	{
	case 0:
		Temp_Ping_State = TEMP_PING_READY;
		break;
	default:
		break;
	}
}

/**
 * @brief Handles incoming temperature requests by reading and sending data.
 */
void App_Temp_RcvRequest(void)
{
	App_Read_Send_Temp_Data();
}

/******************************************************************************/
/* Public APIs */
/******************************************************************************/

/**
 * @brief Initializes and manages the temperature node operations.
 */
void App_NodeTemp_Run()
{
	Node_Config_Data_Struct_type Node_Temp_Cfg =
		{
			.nodeType = NODE_TYPE_TEMPERATURE,
			.threshold = 1};

	MID_ADC_ConfigStruct_type ADC_Cfg_Temp =
		{
			.adcHwUnitId = IP_ADC0,
			.channel = ADC_CHANNEL_12,
			.nodeConfigPtr = &Node_Temp_Cfg,
			.callback = App_Temp_ADC_Notification};
	MID_ADC_Init(&ADC_Cfg_Temp);
	MID_LPIT_Init(LPIT_INS_0, LPIT_Callback_Temp);
	MID_CAN_Init(FlexCAN0_INS);
	MID_CAN_UserConfigType UserCfgMBSendData = {
		.HandlerFunc = NULL,
		.MbID = 0x11,
		.MbIndex = MB0,
		.MbInt = true};

	MID_CAN_UserConfigType UserCfgMBSendPing = {
		.HandlerFunc = NULL,
		.MbID = 0x33,
		.MbIndex = MB1,
		.MbInt = true};

	MID_CAN_StdTxMbInit(FlexCAN0_INS, &UserCfgMBSendData);
	MID_CAN_StdTxMbInit(FlexCAN0_INS, &UserCfgMBSendPing);

	MID_CAN_UserConfigType UserCfgMBRequest = {
		.HandlerFunc = App_Temp_RcvRequest,
		.HandlerType = MIDDLE_HANDLER_MB_2_TYPE,
		.MbID = 0x55,
		.MbIndex = MB2,
		.MbInt = true};
	MID_CAN_StdRxMbInit(FlexCAN0_INS, &UserCfgMBRequest);
	MID_CAN_SetCallback(FlexCAN0_INS, &UserCfgMBRequest);

	MID_LPIT_StartTimer(LPIT_INS_0, 0, 6000000);

	App_Read_Send_Temp_Data();
	while (1)
	{
		App_ProcessTempPing(&Temp_Ping_State);
		App_CheckTempConnect();
		App_TempReconnect();
	}
}
