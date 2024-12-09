/*
 * Mid_Lpit.c
 *
 *  Created on: Nov 7, 2024
 *      Author: HI
 */
/*******************************************************************************
* Include
*******************************************************************************/
#include "../src/middleware/lpit_middleware/src/Mid_Lpit.h"

/*******************************************************************************
* Variables
*******************************************************************************/

/* This variable allows the user to configure out function for LPIT peripheral */
//LpitChannelType LPIT_Channel_Config[] = {LPIT_CHANNEL_0};
//LpitInterruptType LPIT_IntChannel[] = {LPIT_IRQ_CHANNEL_0};

LpitChannelType LPIT_Channel_Config[] = {LPIT_CHANNEL_0};
LpitInterruptType LPIT_IntChannel[] = {LPIT_IRQ_CHANNEL_0};

LpitConfigType LPIT_Config = {
        .debug_en       = LPIT_DEBUG_MODE_ENABLE,
        .doze_en        = LPIT_DOZE_MODE_ENABLE,
        .channel        = LPIT_Channel_Config,
        .num_of_channel = (sizeof(LPIT_Channel_Config) / sizeof(LPIT_Channel_Config[0])),
        .timer_mode     = LPIT_PERIODIC_CNT,
        .interrupt      = LPIT_IntChannel,
        .num_of_interrupt = (sizeof(LPIT_IntChannel) / sizeof(LPIT_IntChannel[0]))
};

/*******************************************************************************
* Api
*******************************************************************************/

/**
  * @brief  This function enables bus clock and config asynchnorous clock for LPIT peripheral
  * @param  None
  * @retval None
  */
static void MID_LPIT_InitClock(void);

/**
  * @brief  This function allows to interrupt LPIT channel in NVIC
  * @param  None
  * @retval None
  */
static void MID_LPIT_EnableInterruptChannel(void);

/*******************************************************************************
* Code
*******************************************************************************/
void MID_LPIT_Init(LpitInsType Lpit_ins, Lpit0_Callback_t LpitCallbackFunc)
{
    /* Initialize clock for LPIT */
    MID_LPIT_InitClock();
    /* save callback function address to config variable */
    LPIT_Config.callback = LpitCallbackFunc;
	/* Init lpit peripheral */
	DRV_LPIT_Init(Lpit_ins, &LPIT_Config);
	/* Enable interrupt in NVIC */
	if (LPIT_Config.num_of_interrupt != 0U)
	{
	    MID_LPIT_EnableInterruptChannel();
	}
	else {}
}

void MID_LPIT_StartTimer(LpitInsType Lpit_ins, LpitChannelType channel, int64_t time_value)
{
	DRV_LPIT_StartTimer(Lpit_ins, channel, time_value);
}

void MID_LPIT_StopTimer(LpitInsType Lpit_ins, LpitChannelType channel)
{
	DRV_LPIT_StopTimer(Lpit_ins, channel);
}

void MID_LPIT_Deinit(LpitInsType Lpit_ins)
{
	DRV_LPIT_Deinit(Lpit_ins);
}

static void MID_LPIT_InitClock(void)
{
    PCC_PeriClockControl(PCC_LPIT_INDEX, CLOCK_FIRCDIV2_CLK, CLOCK_DIV_1, ENABLE);
}

static void MID_LPIT_EnableInterruptChannel(void)
{
    uint8_t idx = 0U;
    /* Enable interrupt on NVIC for LPIT channel */
    for (idx = 0U; idx <= LPIT_Config.num_of_interrupt; idx++)
    {
        switch ((LPIT_Config.interrupt)[idx])
        {
            case LPIT_IRQ_CHANNEL_0:
                NVIC_EnableIRQn(LPIT0_Ch0_IRQn);
                break;
            case LPIT_IRQ_CHANNEL_1:
                NVIC_EnableIRQn(LPIT0_Ch1_IRQn);
                break;
            case LPIT_IRQ_CHANNEL_2:
                NVIC_EnableIRQn(LPIT0_Ch2_IRQn);
                break;
            case LPIT_IRQ_CHANNEL_3:
                NVIC_EnableIRQn(LPIT0_Ch3_IRQn);
                break;
            default:
                break;
        }
    }
}

/*******************************************************************************
* end of file
*******************************************************************************/
