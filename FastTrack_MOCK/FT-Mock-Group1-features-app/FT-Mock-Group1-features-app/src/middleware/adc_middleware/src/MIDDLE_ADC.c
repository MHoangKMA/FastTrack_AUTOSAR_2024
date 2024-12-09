/**
*   @file    ADC_MiddleWare.c
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
#include "../src/middleware/adc_middleware/include/MIDDLE_ADC.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define MAX_TEMPERATURE      (50U)
#define MAX_SPEED            (200U)
#define MAX_MV_OUT           (4095U)


/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

static ADC_Middleware_Callback s_adcMiddlewareCallback = NULL;
static volatile uint16_t s_lastReadData = 0;
static volatile uint16_t s_lastValidData = 0;
static Node_Config_Data_Struct_type *s_nodeConfigPtr = NULL;

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

static uint16_t MID_ADC_ConvertData(uint32_t dataOrigin);
static bool MID_ADC_ValidateDataStep(uint16_t dataConverted);
static void DRV_ADC_Driver_CallBack(uint16_t dataOrigin);

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/**
* @brief
* @details       This function will convert data origin to current data type (speed or temperature)
*
* @param[in]     uint8_t Data_origin
* @retval        data (which had been converted to speed or temperature)
*
* @api
*/
static uint16_t MID_ADC_ConvertData(uint32_t dataOrigin)
{
    uint16_t data = 0;

    if(NODE_TYPE_SPEEED == s_nodeConfigPtr->nodeType)
    {
        data = (uint16_t)((dataOrigin * MAX_SPEED) / MAX_MV_OUT);
    }
    else if(NODE_TYPE_TEMPERATURE == s_nodeConfigPtr->nodeType)
    {
        data = (uint16_t)((dataOrigin * MAX_TEMPERATURE) / MAX_MV_OUT);
    }
    else
    {
        /* Do nothing */
    }
    s_lastReadData = data;

    return data;
}

/**
* @brief
* @details        This function will compare new valid data with threshold value
*
* @param[in]      uint16_t Data_converted
* @return         void.
*
* @api
*/
static bool MID_ADC_ValidateDataStep(uint16_t dataConverted)
{
    bool retVal = false;
    uint16_t step = 0;

    step = (dataConverted > s_lastValidData) ? (dataConverted - s_lastValidData) : (s_lastValidData - dataConverted);
    if (step > s_nodeConfigPtr->threshold)
    {
        retVal = true;
        s_lastValidData = dataConverted;
    }

    return retVal;
}

static void DRV_ADC_Driver_CallBack(uint16_t dataOrigin)
{
    uint16_t dataConverted = 0;

    dataConverted = MID_ADC_ConvertData((uint32_t)dataOrigin);
    if (MID_ADC_ValidateDataStep(dataConverted))
    {
        if (s_adcMiddlewareCallback != NULL)
        {
            s_adcMiddlewareCallback(dataConverted);
        }
    }
    else
    {
        /* Do nothing */
    }
}

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
* @brief
* @details        This function will support to init some information for ADC hardware
*                 to send
*
* @param[in]      PushEvent CallBackEventFunction: notification call back for application
* @return         E_OK.
*                 E_NOT_OK
*
* @api
*/
MID_ADC_ReturnCode_type MID_ADC_Init(MID_ADC_ConfigStruct_type *adcConfig)
{
	PCC_PeriClockControl(PCC_ADC0_INDEX, CLOCK_FIRCDIV2_CLK, CLOCK_DIV_1, ENABLE);
    MID_ADC_ReturnCode_type retVal = ADC_MIDDLEWARE_RETURN_CODE_ERROR;

    s_adcMiddlewareCallback = adcConfig->callback;
    s_nodeConfigPtr = adcConfig->nodeConfigPtr;

    if(ADC_DRIVER_RETURN_CODE_SUCCESSED == DRV_ADC_Init(adcConfig->adcHwUnitId, adcConfig->channel, DRV_ADC_Driver_CallBack))
    {
        DRV_ADC_EnableIRQ(adcConfig->adcHwUnitId, adcConfig->channel);
        retVal = ADC_MIDDLEWARE_RETURN_CODE_SUCCESSED;
    }

    return retVal;
}

uint16_t MID_ADC_ReadData(void)
{
    return s_lastReadData;
}

#ifdef __cplusplus
}
#endif
