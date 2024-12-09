/*******************************************************************************
* Include
*******************************************************************************/
#include "Driver_Header.h"
#include "Lpit_hw_access.h"

/*******************************************************************************
* Variables
*******************************************************************************/

/* Declare array stores addresses of LPIT peripheral instances */
static LPIT_Type * LPIT_Base_Addr[LPIT_INSTANCE_COUNT] = IP_LPIT_BASE_PTRS;
/* Function pointer store address callback function */
static Lpit0_Callback_t s_callback;

/*******************************************************************************
* Code
*******************************************************************************/

LpitStatusType DRV_LPIT_Init(LpitInsType Lpit_ins, const LpitConfigType * UserConfig)
{
    LpitStatusType statusFunc = SUCCESS;
    uint8_t idx = 0;

    if ((UserConfig != DRV_LPIT_NULL_PTR) && (Lpit_ins == LPIT_INS_0))
    {
        LPIT_Type * lpitBase = LPIT_Base_Addr[Lpit_ins];

        /* Reset the timer channels and registers */
        Lpit_ResetTimer(lpitBase);

        /* Setup timer operation in debug and doze modes and enable clock for module */
        Lpit_SetupTimer(lpitBase, UserConfig->debug_en, UserConfig->doze_en);

        /************************/
        for (idx = 0U; idx <= UserConfig->num_of_channel; idx++)
        {
            /* Setup the channel counters operation mode to "32-bit Periodic Counter"
             * and keep default values for the trigger source */
            Lpit_SetTimerOperationMode(lpitBase, (UserConfig->channel)[idx], UserConfig->timer_mode);

            /* Timer stop on timeout */
            Lpit_SetTimerStopOnInterrupt(lpitBase, (UserConfig->channel)[idx], UserConfig->timer_stop_on_interrup);
        }

        /* Enable channel interrupt */
        if (UserConfig->num_of_interrupt != 0U)
        {
            for (idx = 0U; idx <= UserConfig->num_of_interrupt; idx++)
            {
                Lpit_EnableChannelInterrupt(lpitBase, (UserConfig->interrupt)[idx]);
            }
            /* Set call back function when interrupt occur */
            s_callback = UserConfig->callback;
        }
    }
    else
    {
        statusFunc = FAIL;
    }

    return statusFunc;
}

LpitStatusType DRV_LPIT_StartTimer(LpitInsType Lpit_ins, LpitChannelType channel, int64_t timer_value)
{
    LpitStatusType statusFunc = SUCCESS;

    if ((timer_value < 0) || ((uint64_t)timer_value > UINT32_MAX) || (Lpit_ins != LPIT_INS_0) ||
        (channel < LPIT_CHANNEL_0) || (channel > LPIT_CHANNEL_3))
    {
        statusFunc = FAIL;
    }
    else
    {
        LPIT_Type * lpitBase = LPIT_Base_Addr[Lpit_ins];
        /* Set timer value for channel */
        Lpit_SetTimerValue(lpitBase, channel, timer_value);

        /* Starts the timer counting afer all configuration */
        Lpit_EnableTimer(lpitBase, channel);
    }

    return statusFunc;
}

LpitStatusType DRV_LPIT_StopTimer(LpitInsType Lpit_ins, LpitChannelType channel)
{
    LpitStatusType statusFunc = SUCCESS;

    if ((Lpit_ins != LPIT_INS_0) || (channel < LPIT_CHANNEL_0) || (channel > LPIT_CHANNEL_3))
    {
        statusFunc = FAIL;
    }
    else
    {
        LPIT_Type * lpitBase = LPIT_Base_Addr[Lpit_ins];

        Lpit_DisableTimer(lpitBase, channel);
    }

    return statusFunc;
}

/* Interrupt service routine for LPIT0 - channle 0 */
void LPIT0_Ch0_IRQHandler(void)
{
    if (Lpit_CheckInterruptFlagChannel(LPIT_CHANNEL_0))
    {
        /* Clear Interrupt Flag */
        Lpit_ClearInterruptFlagChannel(LPIT_CHANNEL_0);
        /* Call App_Lpit_Callback */
        (*s_callback)(LPIT_CHANNEL_0);
    }
}

/* Interrupt service routine for LPIT0 - channle 1 */
void LPIT0_Ch1_IRQHandler(void)
{
    if (Lpit_CheckInterruptFlagChannel(LPIT_CHANNEL_1))
    {
        /* Clear Interrupt Flag */
        Lpit_ClearInterruptFlagChannel(LPIT_CHANNEL_1);
        /* Call App_Lpit_Callback */
        (*s_callback)(LPIT_CHANNEL_1);
    }
    else {}
}

/* Interrupt service routine for LPIT0 - channle 2 */
void LPIT0_Ch2_IRQHandler(void)
{
    if (Lpit_CheckInterruptFlagChannel(LPIT_CHANNEL_2))
    {
        /* Clear Interrupt Flag */
        Lpit_ClearInterruptFlagChannel(LPIT_CHANNEL_2);
        /* Call App_Lpit_Callback */
        (*s_callback)(LPIT_CHANNEL_2);
    }
    else {}
}

/* Interrupt service routine for LPIT0 - channle 3 */
void LPIT0_Ch3_IRQHandler(void)
{
    if (Lpit_CheckInterruptFlagChannel(LPIT_CHANNEL_3))
    {
        /* Clear Interrupt Flag */
        Lpit_ClearInterruptFlagChannel(LPIT_CHANNEL_3);
        /* Call App_Lpit_Callback */
        (*s_callback)(LPIT_CHANNEL_3);
    }
    else {}
}

LpitStatusType DRV_LPIT_Deinit(LpitInsType Lpit_ins)
{
    LpitStatusType statusFunc = SUCCESS;

    if (Lpit_ins != LPIT_INS_0)
    {
        statusFunc = FAIL;
    }
    else
    {
        LPIT_Type * lpitBase = LPIT_Base_Addr[Lpit_ins];
        /* Reset the timer channels and registers */
        Lpit_ResetTimer(lpitBase);

        /* Disable interrupt on NVIC for LPIT channel */
        NVIC_DisableIRQn(LPIT0_Ch0_IRQn);
        NVIC_DisableIRQn(LPIT0_Ch1_IRQn);
        NVIC_DisableIRQn(LPIT0_Ch2_IRQn);
        NVIC_DisableIRQn(LPIT0_Ch3_IRQn);

        /* Set call back function when interrupt occur */
        s_callback = DRV_LPIT_NULL_PTR;
    }

    return statusFunc;
}

/*******************************************************************************
* end of file
*******************************************************************************/