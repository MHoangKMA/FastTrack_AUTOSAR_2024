/*
 * Drv_Lpit_hw_access.h
 *
 *  Created on: Nov 11, 2024
 *      Author: HI
 */
#ifndef _LPIT_HW_ACCESS_H_
#define _LPIT_HW_ACCESS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <lpit_driver/include/Drv_Lpit.h>

/*******************************************************************************
* Api
*******************************************************************************/

/**
  * @brief  This function allows reset timer before configuration
  * @param  LPIT_Type * LPITx       < pointer access to LPIT instance >
  * @retval None
  */
static inline void Lpit_ResetTimer(LPIT_Type * LPITx)
{
    LPITx->MCR |= LPIT_MCR_SW_RST(1);
    LPITx->MCR &= ~LPIT_MCR_SW_RST_MASK;
}

/**
  * @brief  This function allows setup timer
  * @param  LPIT_Type * LPITx       < pointer access to LPIT instance >
  * @param  uint8_t debugEn         < option enable or disable debug mode >
  * @param  uint8_t dozeEn          < option enable or disable dozeEn mode >
  * @retval None
  */
static inline void Lpit_SetupTimer(LPIT_Type * LPITx, uint8_t debugEn, uint8_t dozeEn)
{
    LPITx->MCR &= (~LPIT_MCR_DBG_EN_MASK) & (~LPIT_MCR_DOZE_EN_MASK);
    LPITx->MCR |= (LPIT_MCR_DBG_EN(debugEn) | LPIT_MCR_DOZE_EN(dozeEn) | LPIT_MCR_M_CEN_MASK);
}

/**
  * @brief  This function allows select mode operation counter
  * @param  LPIT_Type * LPITx       < pointer access to LPIT instance >
  * @param  uint8_t channel         < channel is selected >
  * @param  uint8_t timerMode       < channel counters operation mode >
  * @retval None
  */
static inline void Lpit_SetTimerOperationMode(LPIT_Type * LPITx, uint8_t channel, uint8_t timerMode)
{
    LPITx->TMR[channel].TCTRL = 0;
    LPITx->TMR[channel].TCTRL |= LPIT_TMR_TCTRL_MODE(timerMode);
}

/**
  * @brief  This function allows timer stop or not on interrupt
  * @param  LPIT_Type * LPITx       < pointer access to LPIT instance >
  * @param  uint8_t channel         < channel is selected >
  * @param  uint8_t timerStop       < option stop or not on interrupt >
  * @retval None
  */
static inline void Lpit_SetTimerStopOnInterrupt(LPIT_Type * LPITx, uint8_t channel, uint8_t timerStop)
{
	LPITx->TMR[channel].TCTRL = (LPITx->TMR[channel].TCTRL & ~LPIT_TMR_TCTRL_TSOI_MASK) |
                                LPIT_TMR_TCTRL_TSOI(timerStop);
}

/**
  * @brief  This function allows enable interrupt for channel
  * @param  LPIT_Type * LPITx       < pointer access to LPIT instance >
  * @param  uint8_t interrupt       < interrupt channel is selected >
  * @retval None
  */
static inline void Lpit_EnableChannelInterrupt(LPIT_Type * LPITx, uint8_t interrupt)
{
		LPITx->MIER |= interrupt ;
}

/**
  * @brief  This function allows set timer value for counter
  * @param  LPIT_Type * LPITx       < pointer access to LPIT instance >
  * @param  uint8_t channel         < channel is selected >
  * @param  uint32_t timer_value    < time count >
  * @retval None
  */
static inline void Lpit_SetTimerValue(LPIT_Type * LPITx, uint8_t channel, uint32_t timer_value)
{
    LPITx->TMR[channel].TVAL = timer_value - 1;
}

/**
  * @brief  This function allows start timer to count
  * @param  LPIT_Type * LPITx       < pointer access to LPIT instance >
  * @param  uint8_t channel         < channel is selected >
  * @retval None
  */
static inline void Lpit_EnableTimer(LPIT_Type * LPITx, uint8_t channel)
{
    LPITx->TMR[channel].TCTRL |= LPIT_TMR_TCTRL_T_EN(1);
}

/**
  * @brief  This function allows stop counter
  * @param  LPIT_Type * LPITx       < pointer access to LPIT instance >
  * @param  uint8_t channel         < channel is selected >
  * @retval None
  */
static inline void Lpit_DisableTimer(LPIT_Type * LPITx, uint8_t channel)
{
    LPITx->TMR[channel].TCTRL &= ~LPIT_TMR_TCTRL_T_EN_MASK;
}

/**
  * @brief  This function allows check interrupt flag for channel
  * @param  None
  * @retval None
  */
static inline uint8_t Lpit_CheckInterruptFlagChannel(uint8_t channel)
{
    return (((IP_LPIT0->MSR) & (1U << channel)) >> channel);
}

/**
  * @brief  This function allows clear interrupt flag for channel
  * @param  None
  * @retval None
  */
static inline void Lpit_ClearInterruptFlagChannel(uint8_t channel)
{
    IP_LPIT0->MSR |= 1U << channel;
}

#endif /* _LPIT_HW_ACCESS_H_ */

/*******************************************************************************
* end of file
*******************************************************************************/
