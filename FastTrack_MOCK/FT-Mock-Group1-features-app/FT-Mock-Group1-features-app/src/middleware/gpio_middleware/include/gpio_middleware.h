#include "Driver_Header.h"

/*==================================================================================================
*                                        ENUMS
==================================================================================================*/

/**
 * @brief Enumeration of LED colors.
 *
 * Defines the LED color values used to control the LEDs.
 */
typedef enum
{
    GREEN = 112U,  /*!< Value for the GREEN LED color */
    RED = 111U,    /*!< Value for the RED LED color */
    BLUE = 96U,    /*!< Value for the BLUE LED color */
} LED_Color_t;

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

/**
 * @brief  Initializes the GPIO (General Purpose Input/Output) module.
 *
 * This function configures the necessary GPIO settings to enable communication with the LEDs.
 */
void MID_GPIO_Init(void);

/**
 * @brief  Initializes the LED specified by the color.
 *
 * This function sets up the specified LED (by color) for use, enabling the necessary GPIO pins.
 *
 * @param[in] Color  The LED color to initialize (e.g., GREEN, RED, BLUE).
 */
void MID_GPIO_LEDInit(LED_Color_t Color);

/**
 * @brief  Turns on the LED specified by the color.
 *
 * This function turns on the specified LED (by color) by setting the corresponding GPIO pin.
 *
 * @param[in] Color  The LED color to turn on (e.g., GREEN, RED, BLUE).
 */
void MID_GPIO_LEDOn(LED_Color_t Color);

/**
 * @brief  Turns off the LED specified by the color.
 *
 * This function turns off the specified LED (by color) by clearing the corresponding GPIO pin.
 *
 * @param[in] Color  The LED color to turn off (e.g., GREEN, RED, BLUE).
 */
void MID_GPIO_LEDOff(LED_Color_t Color);
