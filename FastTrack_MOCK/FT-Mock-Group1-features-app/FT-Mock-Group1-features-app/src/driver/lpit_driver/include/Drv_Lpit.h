/*
 * Adc.h
 *
 *  Created on: May 5, 2024
 *      Author: BienTD
 */
#ifndef _DRV_LPIT_H_
#define _DRV_LPIT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "S32K144.h"

/*******************************************************************************
* Definitions
*******************************************************************************/

#define DRV_LPIT_NULL_PTR       ((void*)0)
#define DRV_LPIT_MAX_CHANNEL    3U

/* Define function pointer use when callback function */
typedef void (*Lpit0_Callback_t)(uint8_t);

/* Enum stores the channels of LPIT */
typedef enum
{
    LPIT_CHANNEL_0 = 0U, /*!< LPIT channel number 0*/
    LPIT_CHANNEL_1,      /*!< LPIT channel number 1 */
    LPIT_CHANNEL_2,      /*!< LPIT channel number 2 */
    LPIT_CHANNEL_3,      /*!< LPIT channel number 3 */
} LpitChannelType;

/* Enum stores the timer mode of LPIT */
typedef enum
{
    LPIT_PERIODIC_CNT = 0U,     /*!< Use the all 32-bits, counter loads and decrements to zero */
    LPIT_DUAL_PERIODIC_CNT,     /*!< Counter loads, lower 16-bits  decrement to zero, then upper 16-bits  decrement */
    LPIT_TRIGGER_ACCU,          /*!< Counter loads on first trigger and decrements on each trigger */
    LPIT_INPUT_CAPTURE          /*!< Counter  loads with 0xFFFFFFFF, decrements to zero. It stores
                                the inverse of the current value when a input trigger is detected */
} LpitTimerModeType;

/* Enum stores trigger options for LPIT channels */
typedef enum
{
    LPIT_TRIGGER_TIM_CHANNEL_0 = 0U, /*!< Channel 0 is selected as a trigger source */
    LPIT_TRIGGER_TIM_CHANNEL_1,      /*!< Channel 1 is selected as a trigger source */
    LPIT_TRIGGER_TIM_CHANNEL_2,      /*!< Channel 2 is selected as a trigger source */
    LPIT_TRIGGER_TIM_CHANNEL_3,      /*!< Channel 3 is selected as a trigger source */
} LpitTriggerSelectType;

/* Enum stores trigger sources for LPIT */
typedef enum
{
    LPIT_TRIGGER_SOURCE_EXT = 0U,   /*!< Use external trigger input */
    LPIT_TRIGGER_SOURCE_IN          /*!< Use internal trigger */
} LpitTriggerSourceType;

/* Enum stores the interrupt options for the channel */
typedef enum
{
    LPIT_IRQ_DISABLE    = 0,
    LPIT_IRQ_CHANNEL_0  = (1U << 0), /*!< Channel 0 Timer interrupt */
    LPIT_IRQ_CHANNEL_1  = (1U << 1), /*!< Channel 1 Timer interrupt */
    LPIT_IRQ_CHANNEL_2  = (1U << 2), /*!< Channel 2 Timer interrupt */
    LPIT_IRQ_CHANNEL_3  = (1U << 3), /*!< Channel 3 Timer interrupt */
} LpitInterruptType;

/* Enum stores toggle options for Debug mode */
typedef enum
{
    LPIT_DEBUG_MODE_DISABLE = 0U,
    LPIT_DEBUG_MODE_ENABLE  = 1U
} LpitDebugModeType;

/* Enum stores toggle options for Doze mode */
typedef enum
{
    LPIT_DOZE_MODE_DISABLE  = 0U,
    LPIT_DOZE_MODE_ENABLE   = 1U
} LpitDozeModeType;

/* Enum stores options timer stop on interrupt */
typedef enum
{
    LPIT_TSOI_DISABLE,      /* Timer doesn't stop on interrupt */
    LPIT_TSOI_ENABLE        /* Timer stop on interrupt */
} LpitTimerStopOnInterrupType;

/* Struct stores lpit configuration options for the user */
typedef struct
{
    LpitDebugModeType debug_en;
    LpitDozeModeType doze_en;
    LpitChannelType * channel;
    uint8_t num_of_channel;
    LpitTimerModeType timer_mode;
    LpitInterruptType * interrupt;
    LpitTimerStopOnInterrupType timer_stop_on_interrup;
    uint8_t num_of_interrupt;
    Lpit0_Callback_t callback;
} LpitConfigType;

/* enum stores Lpit peripheral instances */
typedef enum {
    LPIT_INS_0 = 0U,
} LpitInsType;

typedef enum {
    SUCCESS,
    FAIL
} LpitStatusType;

/*******************************************************************************
* Api
*******************************************************************************/

/**
  * @brief  This function function allows init LPIT peripheral
  * @param  LpitInsType Lpit_ins         < LPIT peripheral instance >
  * @param  LpitConfigType * UserConfig  < The struct variable stores the
  *                                        user's configuration >
  * @retval LpitStatusType: - SUCCESS    < Initialization successful >
  *                         - FAIL       < Initialization failed >
  */
LpitStatusType DRV_LPIT_Init(LpitInsType Lpit_ins, const LpitConfigType * UserConfig);

/**
  * @brief  This function function allows starting LPIT timer
  * @param  LpitInsType Lpit_ins         < LPIT peripheral instance >
  * @param  LpitChannelType channel      < channel is selected >
  * @param  uint32_t timer_value         < counting time for counter >
  * @retval LpitStatusType: - SUCCESS    < Successful action >
  *                         - FAIL       < Failed action >
  */
LpitStatusType DRV_LPIT_StartTimer(LpitInsType Lpit_ins, LpitChannelType channel, int64_t timer_value);

/**
  * @brief  This function function allows stop LPIT timer
  * @param  LpitInsType Lpit_ins         < LPIT peripheral instance >
  * @param  LpitChannelType channel      < channel is selected >
  * @retval LpitStatusType: - SUCCESS    < Successful action >
  *                         - FAIL       < Failed action >
  */
LpitStatusType DRV_LPIT_StopTimer(LpitInsType Lpit_ins, LpitChannelType channel);

/**
  * @brief  This function function allows deinit LPIT peripheral
  * @param  LpitInsType Lpit_ins         < LPIT peripheral instance >
  * @param  LpitChannelType channel      < channel is selected >
  * @retval LpitStatusType: - SUCCESS    < Successful action >
  *                         - FAIL       < Failed action >
  */
LpitStatusType DRV_LPIT_Deinit(LpitInsType Lpit_ins);

#endif /* _DRV_LPIT_H_ */

/*******************************************************************************
* end of file
*******************************************************************************/