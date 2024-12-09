/**
*   @file    ADC_Driver.c
*
*   @version 1.0.0
*
*   @addtogroup ADC
*   @{
*/
#ifdef __cplusplus
extern "C" {
#endif
/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "ADC_Driver.h"
#include <stddef.h>

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
/**
* @brief          Maximum Precision channel for ADC0 range from 0 to 15
*/
#define ADC0_ST_PRICISION_CH_U8           (0U)
#define ADC0_ED_PRICISION_CH_U8           (15U)
#define ADC0_INVALID_CHANEL               (ADC_Channel_type)(16U)


/**
 * @brief          Maximum Precision channel for ADC1 range from 0 to 9
 */
#define ADC1_ST_PRICISION_CH_U8           (0U)
#define ADC1_ED_PRICISION_CH_U8           (9U)
#define ADC1_INVALID_CHANEL               (ADC_Channel_type)(10U)

/* Register to pick data */
#define CURRENT_DATA_RESULT_REG           (4U)
/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
static volatile uint16_t s_latestData = 0;
static IRQCallBack ADC_CallBack = NULL;
static uint8_t s_currentChannel = 0;

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


static void DRV_ADC_ModuleConfig(ADC_Type * adcHwUnitId, ADC_Channel_type channel, ADC_ModuleConfig_type *pConfig);
static void DRV_PDB_ModuleConfig(PDB_Type *PDBTarget, uint8_t PDBIndex);

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


static void DRV_ADC_ModuleConfig(ADC_Type * adcHwUnitId, ADC_Channel_type channel, ADC_ModuleConfig_type *pConfig)
{
    adcHwUnitId->SC3 = ADC_SC3_CAL_MASK | ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(pConfig->hwAvrgSelect);
    /* Wait for completion */
    while(((adcHwUnitId->SC1[0] & ADC_SC1_COCO_MASK) >> ADC_SC1_COCO_SHIFT) == 0)
    {};
    /* ADCH: Module disabled for conversions */
    adcHwUnitId->SC1[CURRENT_DATA_RESULT_REG] = ADC_SC1_ADCH_MASK;
    /* ADIV = 0: Divide ratio = 1
    MODE = 1: 12-bit conversion */
    adcHwUnitId->CFG1 = ADC_CFG1_ADIV(pConfig->clockDivide) | ADC_CFG1_MODE(pConfig->convMode);
    /* SMPLTS = 12: sample time is 13 ADC clks */
    adcHwUnitId->CFG2 = ADC_CFG2_SMPLTS(pConfig->sampleTime);
    /* ADTRG = 1: HW trigger */
    adcHwUnitId->SC2 = ADC_SC2_ADTRG(pConfig->trigger);
    /* ADCH = channel: External channel as ADC0 input */
    adcHwUnitId->SC1[CURRENT_DATA_RESULT_REG] = ADC_SC1_ADCH(channel);
    /* ADCO = 0: One conversion performed
       AVGE,AVGS = 0: HW average function disabled */
    //adcHwUnitId->SC3 = 0x00000000;
    adcHwUnitId->SC3 = ADC_SC3_CAL(ADC_CALIBRATION_DISABLE) 
                     | ADC_SC3_AVGE(ADC_HW_AVRG_DISABLE) 
                     | ADC_SC3_ADCO(pConfig->convType)
                     | ADC_SC3_AVGS(ADC_HW_4_SAMPLES_AVRG);
    
}

static void DRV_PDB_ModuleConfig(PDB_Type *PDBTarget, uint8_t PDBIndex)
{
/* static void PDB_BusClockEnable(void) */
    /* Enable bus clock in PDB */
	PCC_PeriClockControl(PDBIndex, CLOCK_NOSRC_CLK, CLOCK_DIV_DISABLED, ENABLE);
/* static void PDB_PeriodConfig(PDB_SC_Type *pConfig) */
    /* PRESCALER = 6: clk divided by (64 x Mult factor) */
    PDBTarget->SC = PDB_SC_PRESCALER(PDB_PRESCALER_DIV_64)
    | PDB_SC_TRGSEL(PDB_TRIGGER_SOFTWARE) /* TRGSEL = 15: Software trigger selected */
    | PDB_SC_MULT(PDB_MULT_FACTOR_40) /* MULT = 3: Multiplication factor is 40 */
    | PDB_SC_CONT_MASK; /* CONT = 1: Enable operation in continuous mode */
    /* PDB Period = (System Clock / (Prescaler x Mult factor)) / Modulus */
    /* PDB Period = (48 MHz / (64 x 40)) / 18750 */
    /* PDB Period = (18750 Hz) / (18750) = 1 Hz */
    PDBTarget->MOD = 1875;

/* static void PDB_ChannelConfig(PDB_ChannelConfig_Type *pConfig) */
    /* TOS = 10h: Pre-trigger 4 asserts with DLY match */
    /* EN = 10h: Pre-trigger 4 enabled */
    PDBTarget->CH[0].C1 = (PDB_C1_TOS(0x10) | PDB_C1_EN(0x10));
    /* Delay set to half the PDB period = 9375 */
    PDBTarget->CH[0].DLY[4] = 937;

/* static void PDB_Enable(void) */
    /* Enable PDB. Load MOD and DLY */
    PDBTarget->SC |= PDB_SC_PDBEN_MASK | PDB_SC_LDOK_MASK;
    /* Single initial PDB trigger */
    PDBTarget->SC |= PDB_SC_SWTRIG_MASK;
}

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
* @brief          Initializes the ADC hardware unit and the driver.
* @details        This function will initialize both the ADC HW unit and the driver structures.
*                 Enables the hardware trigger for the requested ADC0/ADC1
*
* @param[in]      adcHwUnitId - ADC0 or ADC1 selection
*                 channel     - select external channel set as input to read ADC data
*                 CallBackFunction
* @return         - ADC_DRIVER_RETURN_CODE_SUCCESSED
*                 - ADC_DRIVER_RETURN_CODE_ERROR
*
* @post           Initializes the driver.
*
*/
ADC_Driver_ReturnCode_t DRV_ADC_Init(ADC_Type * adcHwUnitId, ADC_Channel_type channel, IRQCallBack CallBackFunction)
{
    ADC_Driver_ReturnCode_t retVal = ADC_DRIVER_RETURN_CODE_ERROR;
    PDB_Type * PDBTarget = IP_PDB0;
    uint8_t PDBIndex = 0;
    ADC_ModuleConfig_type pAdcConfig = 
    {
        .hwAvrgSelect = ADC_HW_32_SAMPLES_AVRG,
        .clockDivide = ADC_CLOCK_DIV_1,
        .convMode = ADC_CONV_MODE_12_BIT,
        .sampleTime = 12u,
        .trigger = ADC_HARDWARE_TRIGGER,
        .convType = ADC_ONE_CONVERSION
    };

/* Check if parameters valid or not */
    if((adcHwUnitId == IP_ADC0) && (channel <= ADC0_ED_PRICISION_CH_U8))
    {
        retVal = ADC_DRIVER_RETURN_CODE_SUCCESSED;
        PDBTarget = IP_PDB0 ;
        PDBIndex = PCC_PDB0_INDEX;
    }
    else if((adcHwUnitId == IP_ADC1) && (channel <= ADC1_ED_PRICISION_CH_U8))
    {
        retVal = ADC_DRIVER_RETURN_CODE_SUCCESSED;
        PDBTarget = IP_PDB1;
        PDBIndex = PCC_PDB1_INDEX;
    }

    if(ADC_DRIVER_RETURN_CODE_SUCCESSED == retVal)
    {
        DRV_ADC_ModuleConfig(adcHwUnitId, channel, &pAdcConfig);
        DRV_PDB_ModuleConfig(PDBTarget, PDBIndex);
        ADC_CallBack = CallBackFunction;
    }

    return retVal;
}

/**
* @brief
* @details        This function will enable interrupt for external channel
*                 to read ADC data depend on signal as PDB trigger
*
* @param[in]      channel           - select external channel set as input to read ADC data
*                 adcHwUnitId       - ADC0 or ADC1 selection
*
* @return         - ADC_DRIVER_RETURN_CODE_SUCCESSED
*                 - ADC_DRIVER_RETURN_CODE_ERROR
*
* @api
*/
ADC_Driver_ReturnCode_t DRV_ADC_EnableIRQ(ADC_Type * adcHwUnitId, ADC_Channel_type channel)
{
    ADC_Driver_ReturnCode_t retVal = ADC_DRIVER_RETURN_CODE_ERROR;
    IRQn_Type irqTarget;

    if((IP_ADC0 == adcHwUnitId) && (channel <= ADC0_ED_PRICISION_CH_U8))
    {
        retVal = ADC_DRIVER_RETURN_CODE_SUCCESSED;
        irqTarget = ADC0_IRQn;
    }
    else if((IP_ADC1 == adcHwUnitId) && (channel <= ADC1_ED_PRICISION_CH_U8))
    {
        retVal = ADC_DRIVER_RETURN_CODE_SUCCESSED;
        irqTarget = ADC1_IRQn;
    }
    else
    {
        /* Do nothing */
    }

    if(ADC_DRIVER_RETURN_CODE_SUCCESSED == retVal)
    {
        s_currentChannel = channel;
        NVIC_EnableIRQn(irqTarget);
#ifndef UNITTEST
        /* all interrupts are allow for activity */
        __asm("cpsie i");
#endif
        /* Select External channel as ADC input, enable interrupt and clear COCO flag if set */
        adcHwUnitId->SC1[CURRENT_DATA_RESULT_REG] = ADC_SC1_ADCH(channel) | ADC_SC1_AIEN_MASK;
    }

    return retVal;
}

/**
* @brief
* @details        ADC0 Interrupt handler.
*
* @param[in]      void.
* @return         void.
*
*/
void ADC0_IRQHandler(void)
{
    s_latestData = IP_ADC0->R[CURRENT_DATA_RESULT_REG];
    if (ADC_CallBack != NULL)
    {
        ADC_CallBack(s_latestData);
    }
}

/**
* @brief
* @details        ADC1 Interrupt handler.
*
* @param[in]      void.
* @return         void.
*
*/
void ADC1_IRQHandler(void)
{
    s_latestData = IP_ADC1->R[CURRENT_DATA_RESULT_REG];
    if (ADC_CallBack != NULL)
    {
        ADC_CallBack(s_latestData);
    }
}

#ifdef __cplusplus
}
#endif
