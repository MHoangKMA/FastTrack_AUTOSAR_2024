/**
*   @file    ADC_MiddleWare.h
*
*   @version 1.0.0
*
*   @addtogroup ADC
*   @{
*/

#ifndef __ADC_MIDDLEWARE_H__
#define __ADC_MIDDLEWARE_H__

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include <stdint.h>
#include "Driver_Header.h"

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/**
 * @brief ADC middleware callback function type.
 *
 * This callback is invoked when ADC data is ready.
 *
 * @param[in] data  The ADC data read from the hardware
 */
typedef void (*ADC_Middleware_Callback)(uint16_t data); /* Definition of ADC call back */

/**
 * @brief Return code for ADC middleware functions.
 */
typedef enum{
    ADC_MIDDLEWARE_RETURN_CODE_ERROR     = 0u, /*!< Error return code */
    ADC_MIDDLEWARE_RETURN_CODE_SUCCESSED = 1u  /*!< Success return code */
}MID_ADC_ReturnCode_type;

/**
 * @brief Structure to hold the ADC middleware configuration.
 *
 * Contains the ADC hardware unit ID, channel, callback function, and node configuration pointer.
 */
typedef struct MID_ADC_ConfigStruct_type
{
    ADC_Type *adcHwUnitId;                /*!< Pointer to the ADC hardware unit */
    ADC_Channel_type channel;              /*!< ADC channel to read from */
    ADC_Middleware_Callback callback;      /*!< Callback function when ADC data is ready */
    Node_Config_Data_Struct_type *nodeConfigPtr; /*!< Pointer to node configuration data */
} MID_ADC_ConfigStruct_type;

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

/**
 * @brief  Initialize the ADC middleware.
 *
 * This function configures the ADC hardware unit, channel, and callback function.
 *
 * @param[in]  adcConfig  Pointer to structure containing ADC configuration data.
 *
 * @retval MID_ADC_ReturnCode_type  Return code indicating success or error.
 */
MID_ADC_ReturnCode_type MID_ADC_Init(MID_ADC_ConfigStruct_type *adcConfig);

/**
 * @brief  Read data from the ADC middleware.
 *
 * This function returns the most recent ADC data.
 *
 * @return uint16_t  Data read from the ADC hardware.
 */
uint16_t MID_ADC_ReadData(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_MIDDLEWARE_H__ */
