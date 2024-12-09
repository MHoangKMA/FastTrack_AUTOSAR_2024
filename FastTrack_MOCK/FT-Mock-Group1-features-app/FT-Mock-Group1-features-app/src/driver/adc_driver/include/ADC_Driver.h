/**
*   @file    ADC_Driver.h
*
*   @version 1.0.0
*
*   @addtogroup ADC
*   @{
*/
#ifndef __ADC_DRIVER_H__
#define __ADC_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Driver_Header.h"

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/**
 * @brief Enum type for Adc function return type
 */
typedef enum ADC_Driver_ReturnCode_t
{
    ADC_DRIVER_RETURN_CODE_ERROR     = 0u,
    ADC_DRIVER_RETURN_CODE_SUCCESSED = 1u
} ADC_Driver_ReturnCode_t;

/**
 * @brief Enum type for Input channel select
 */
typedef enum ADC_Channel_t
{
    ADC_CHANNEL_0  = 0u,
    ADC_CHANNEL_1  = 1u,
    ADC_CHANNEL_2  = 2u,
    ADC_CHANNEL_3  = 3u,
    ADC_CHANNEL_4  = 4u,
    ADC_CHANNEL_5  = 5u,
    ADC_CHANNEL_6  = 6u,
    ADC_CHANNEL_7  = 7u,
    ADC_CHANNEL_8  = 8u,
    ADC_CHANNEL_9  = 9u,
    ADC_CHANNEL_10 = 10u,
    ADC_CHANNEL_11 = 11u,
    ADC_CHANNEL_12 = 12u,
    ADC_CHANNEL_13 = 13u,
    ADC_CHANNEL_14 = 14u,
    ADC_CHANNEL_15 = 15u
} ADC_Channel_type;

/**
 * @brief Enum type for Hardware Average Enable
 * @details Enables the hardware average function of the ADC
 */
typedef enum ADC_HwAvrgEnable_t
{
    ADC_HW_AVRG_DISABLE = 0u,   /*< Hardware average function disabled */
    ADC_HW_AVRG_ENABLE  = 1u    /*< Hardware average function enabled  */
} ADC_HwAvrgEnable_type;

/**
 * @brief Enum type for Hardware Average Select
 * @details Determines how many ADC conversions will be averaged to create the ADC average result
 */
typedef enum ADC_HwAvrgSelect_t
{
    ADC_HW_4_SAMPLES_AVRG  = 0u,     /*< 4 samples averaged */
    ADC_HW_8_SAMPLES_AVRG  = 1u,     /*< 8 samples averaged */
    ADC_HW_16_SAMPLES_AVRG = 2u,     /*< 16 samples averaged */
    ADC_HW_32_SAMPLES_AVRG = 3u      /*< 32 samples averaged */
} ADC_HwAvrgSelect_type;

/**
 * @brief Enum type for Clock Divide Select
 * @details Selects the divide ratio used by the ADC to generate the internal clock ADCK
 */
typedef enum ADC_ClkDivideSelect_t
{
    ADC_CLOCK_DIV_1 = 0u,           /*< The divide ratio is 1 */
    ADC_CLOCK_DIV_2 = 1u,           /*< The divide ratio is 2 */
    ADC_CLOCK_DIV_3 = 2u,           /*< The divide ratio is 4 */
    ADC_CLOCK_DIV_4 = 3u            /*< The divide ratio is 8 */
} ADC_ClkDivideSelect_type;

/**
 * @brief Enum type for Conversion mode selection
 * @details Selects the ADC resolution
 */
typedef enum ADC_ConversionModeSelect_t
{
    ADC_CONV_MODE_8_BIT  = 0u,      /*< 8-bit conversion */
    ADC_CONV_MODE_12_BIT = 1u,      /*< 12-bit conversion */
    ADC_CONV_MODE_10_BIT = 2u       /*< 10-bit conversion */
} ADC_ConversionModeSelect_type;

/**
 * @brief Enum type for Input Clock Selection
 * @details Selects the input clock source to generate the internal clock 
 */
typedef enum ADC_InputClockSelect_t
{
    ADC_ALTERMATE_CLOCK_1 = 0u,     /*< Alternate clock 1 (ALTCLK1) */
    ADC_ALTERMATE_CLOCK_2 = 1u,     /*< Alternate clock 2 (ALTCLK2) */
    ADC_ALTERMATE_CLOCK_3 = 2u,     /*< Alternate clock 3 (ALTCLK3) */
    ADC_ALTERAMTE_CLOCK_4 = 3u      /*< Alternate clock 4 (ALTCLK4) */
} ADC_InputClockSelect_type;

typedef uint8_t ADC_SampleTimeSelect_type;  /*< Sample time select (1-255)*/

/**
 * @brief Enum type for Conversion Trigger Select
 * @details Selects the type of trigger used for initiating a conversion
 */
typedef enum ADC_ConvTriggerSelect_t
{
    ADC_SOFTWARE_TRIGGER = 0u,      /*< Software trigger selected */
    ADC_HARDWARE_TRIGGER = 1u       /*< Hardware trigger selected */
} ADC_ConvTriggerSelect_type;

/**
 * @brief Enum type for Continuous Conversion Enable
 */
typedef enum ADC_Conversion_t
{
    ADC_ONE_CONVERSION          = 0u,   /*< One conversion will be performed */
    ADC_CONTINUOUS_CONVERSION   = 1u    /*< Continuous conversions will be performed */
} ADC_Conversion_type;


typedef enum ADC_CalibrationStart_t
{
    ADC_CALIBRATION_DISABLE = 0u,
    ADC_CALIBRATION_ENABLE  = 1u
} ADC_CalibrationStart_type;

typedef struct ADC_ModuleConfig_t
{
    //ADC_CalibrationStart_type calibration;
    ADC_HwAvrgEnable_type hwAvrgEnable;
    ADC_HwAvrgSelect_type hwAvrgSelect;
    ADC_ClkDivideSelect_type clockDivide;
    ADC_ConversionModeSelect_type convMode;
    ADC_SampleTimeSelect_type sampleTime;
    ADC_ConvTriggerSelect_type trigger;
    ADC_Conversion_type convType;
} ADC_ModuleConfig_type;

/**
 * @brief Enum type for Prescaler Divider Select
 * @details Counting uses the peripheral clock divided by the product of a factor 
 *          (selected by MULT field) and an integer factor (set by PRESCALAR field)
 */
typedef enum PDB_PrescalerDivSelect_t
{
    PDB_PRESCALER_DIV_1     = 0u,   /*< peripheral clock divided by MUL */
    PDB_PRESCALER_DIV_2     = 1u,   /*< peripheral clock divided by 2xMUL */
    PDB_PRESCALER_DIV_4     = 2u,   /*< peripheral clock divided by 4xMUL */
    PDB_PRESCALER_DIV_8     = 3u,   /*< peripheral clock divided by 8xMUL */
    PDB_PRESCALER_DIV_16    = 4u,   /*< peripheral clock divided by 16xMUL */
    PDB_PRESCALER_DIV_32    = 5u,   /*< peripheral clock divided by 32xMUL */
    PDB_PRESCALER_DIV_64    = 6u,   /*< peripheral clock divided by 64xMUL */
    PDB_PRESCALER_DIV_128   = 7u    /*< peripheral clock divided by 128xMUL */
} PDB_PrescalerDivSelect_type;

/**
 * @brief Enum type for Trigger Input Source Select
 * @details Selects the trigger input source for the PDB. 
 * The trigger input source can be internal or external (EXTRG pin), or the software trigger
 */
typedef enum PDB_TriggerSourceSelect_t
{
    PDB_TRIGGER_0       = 0u,   /*< Trigger-In 0 is selected. */
    PDB_TRIGGER_1       = 1u,   /*< Trigger-In 1 is selected. */
    PDB_TRIGGER_2       = 2u,   /*< Trigger-In 2 is selected. */
    PDB_TRIGGER_3       = 3u,   /*< Trigger-In 3 is selected. */
    PDB_TRIGGER_4       = 4u,   /*< Trigger-In 4 is selected. */
    PDB_TRIGGER_5       = 5u,   /*< Trigger-In 5 is selected. */
    PDB_TRIGGER_6       = 6u,   /*< Trigger-In 6 is selected. */
    PDB_TRIGGER_7       = 7u,   /*< Trigger-In 7 is selected. */
    PDB_TRIGGER_8       = 8u,   /*< Trigger-In 8 is selected. */
    PDB_TRIGGER_9       = 9u,   /*< Trigger-In 9 is selected. */
    PDB_TRIGGER_10      = 10u,  /*< Trigger-In 10 is selected. */
    PDB_TRIGGER_11      = 11u,  /*< Trigger-In 11 is selected. */
    PDB_TRIGGER_12      = 12u,  /*< Trigger-In 12 is selected. */
    PDB_TRIGGER_13      = 13u,  /*< Trigger-In 13 is selected. */
    PDB_TRIGGER_14      = 14u,  /*< Trigger-In 14 is selected. */
    PDB_TRIGGER_SOFTWARE = 15u, /*< Software trigger is selected */
} PDB_TriggerSourceSelect_type;

typedef enum PDB_ModuleEnable_t
{
    PDB_DISABLE     = 0u,
    PDB_ENABLE      = 1u
} PDB_ModuleEnable_type;

typedef enum PDB_MultiFactorSelect_t
{
    PDB_MULT_FACTOR_1   = 0u,
    PDB_MULT_FACTOR_10  = 1u,
    PDB_MULT_FACTOR_20  = 2u,
    PDB_MULT_FACTOR_40  = 3u
} PDB_MultiFactorSelect_type;

typedef enum PDB_ContiuousMode_t
{
    PDB_MODE_ONE_SHOT   = 0u,
    PDB_MODE_CONTINUOUS = 1u
} PDB_ContiuousMode_type;

typedef uint32_t PDB_Modules_type;


/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef void (*IRQCallBack)(uint16_t dataOrigin);

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/
/**
* @brief          Initializes the ADC hardware unit and the driver.
* @details        This function will initialize both the ADC HW unit and the driver structures.
*
* @param[in]      channel: select external channel set as input to read ADC data
*                 adcHwUnitId as ADC0 or ADC1 selection
*                 CallBackFunction
*
* @return         - ADC_DRIVER_RETURN_CODE_SUCCESSED      All data input is valid
*                 - ADC_DRIVER_RETURN_CODE_ERROR
*
* @api
*
* @post           Initializes the driver.
*/
ADC_Driver_ReturnCode_t DRV_ADC_Init(ADC_Type * AdcHwUnitId, ADC_Channel_type Channel, IRQCallBack CallBackFunction);

/**
* @brief          Enables ADC interrupt for the requested ADC0/ADC1 with Channel 0-15/0-9.
* @details        This function will enable the HW trigger source depend on PDB signal
*
* @param[in]      channel: select external channel set as input to read ADC data
*                 adcHwUnitId as ADC0 or ADC1 selection
*
* @return         - ADC_DRIVER_RETURN_CODE_SUCCESSED      All data input is valid
*                 - ADC_DRIVER_RETURN_CODE_ERROR
*
* @api
*/
ADC_Driver_ReturnCode_t DRV_ADC_EnableIRQ(ADC_Type * AdcHwUnitId, ADC_Channel_type Channel);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_DRIVER_H__ */
