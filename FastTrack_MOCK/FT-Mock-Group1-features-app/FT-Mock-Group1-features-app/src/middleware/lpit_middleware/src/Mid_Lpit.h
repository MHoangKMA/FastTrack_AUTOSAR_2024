/*
 * Mid_Lpit.h
 *
 *  Created on: Nov 7, 2024
 *      Author: HI
 */
#ifndef _MID_LPIT_H_
#define _MID_LPIT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "Driver_Header.h"

/*******************************************************************************
* Api
*******************************************************************************/

/**
  * @brief  This function initializes the LPIT peripheral and sets the callback function for interrupts.
  * @param  LpitInsType Lpit_ins            < LPIT peripheral instance >
  * @param  Lpit0_Callback_t LpitCallbackFunc < Pointer to callback function executed on timer interrupt >
  * @retval None
  */
void MID_LPIT_Init(LpitInsType Lpit_ins, Lpit0_Callback_t LpitCallbackFunc);

/**
  * @brief  This function starts the LPIT timer with a specified time value.
  * @param  LpitInsType Lpit_ins          < LPIT peripheral instance >
  * @param  LpitChannelType channel       < Selected LPIT channel >
  * @param  int64_t time_value            < Time value for the timer >
  * @retval None
  */
void MID_LPIT_StartTimer(LpitInsType Lpit_ins, LpitChannelType channel, int64_t time_value);

/**
  * @brief  This function stops the LPIT timer on the specified channel.
  * @param  LpitInsType Lpit_ins          < LPIT peripheral instance >
  * @param  LpitChannelType channel       < Selected LPIT channel >
  * @retval None
  */
void MID_LPIT_StopTimer(LpitInsType Lpit_ins, LpitChannelType channel);

/**
  * @brief  This function deinitializes the specified LPIT channel.
  * @param  LpitInsType Lpit_ins          < LPIT peripheral instance >
  * @param  LpitChannelType channel       < Selected LPIT channel >
  * @retval None
  */
void MID_LPIT_Deinit(LpitInsType Lpit_ins);

#endif /* LPIT_INC_MID_LPIT_H_ */

/*******************************************************************************
* end of file
*******************************************************************************/