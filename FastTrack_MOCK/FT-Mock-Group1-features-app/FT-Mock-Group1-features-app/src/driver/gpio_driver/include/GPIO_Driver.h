/**
*   @file    GPIO_Driver.h
*
*   @version 1.0.0
*
*   @addtogroup GPIO
*   @{
*/

#ifndef __GPIO_H__
#define __GPIO_H__

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
*                                      DEFINES AND MACROS
==================================================================================================*/

#define PORT_MAX_INSTANCE       5
#define PORTA_INSTANCE          0
#define PORTB_INSTANCE          1
#define PORTC_INSTANCE          2
#define PORTD_INSTANCE          3
#define PORTE_INSTANCE          4
#define PORT_PIN_CODE(port_instance, pin) (((port_instance) << 5) | (pin))

/*==================================================================================================
*                                TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/**
 * @brief Enum to define GPIO level definitions
*/
typedef enum GPIO_Level_t
{
    GPIO_PIN_LEVEL_LOW    = 0U,       /*< logic level 0 / LOW */
    GPIO_PIN_LEVEL_HIGH   = 1U        /*< logic level 1 / HIGH */
} GPIO_Level_type;


/**
 * @brief Enum to define GPIO mode definitions
*/
typedef enum GPIO_Mode_t
{
    GPIO_PIN_MODE_OUTPUT = 0U,       /*< GPIO Ouput mode */
    GPIO_PIN_MODE_INPUT  = 1U        /*< GPIO Input mode */
} GPIO_Mode_type;

/** @brief struct information need to config for a specific GPIO input pin
*/
typedef struct GPIO_PinConfig_t
{
    uint8_t pinCode;               /* pinCode represent both PORT name and pin number */
    GPIO_Mode_type pinMode;        /* GPIO pin mode configure */
} GPIO_PinConfig_type;


/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

/**
 * @brief Set the mode of a specific GPIO pin
 *
 * @param[in] pinConfig  Pointer to the GPIO pin config structure
 *
 * @return void
 */
void GPIO_Driver_ModeConfig(const GPIO_PinConfig_type *pinConfig);

/**
 * @brief Set the output of a specific GPIO pin to HIGH
 *
 * @param[in] pinCode  Pin code represent both PORT name and pin number
 *
 * @return void
 */
void GPIO_Driver_SetOutput(uint8_t pinCode);

/**
 * @brief Clear the output of a specific GPIO pin to LOW
 *
 * @param[in] pinCode  Pin code represent both PORT name and pin number
 *
 * @return void
 */
void GPIO_Driver_ClearOutput(uint8_t pinCode);

/**
 * @brief Toggle the output of a specific GPIO pin
 *
 * @param[in] pinCode  Pin code represent both PORT name and pin number
 *
 * @return void
 */
void GPIO_Driver_ToggleOutput(uint8_t pinCode);

/**
 * @brief Read the level of a specific GPIO pin
 *
 * @param[in] pinCode  Pin code represent both PORT name and pin number
 *
 * @return GPIO level type
 */
GPIO_Level_type GPIO_Driver_ReadPin(uint8_t pinCode);

/**
 * @brief Write a level to a specific GPIO pin
 *
 * @param[in] pinCode  Pin code represent both PORT name and pin number
 * @param[in] level    GPIO level type
 *
 * @return void
 */
void GPIO_Driver_WritePin(uint8_t pinCode, const GPIO_Level_type level);


#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
