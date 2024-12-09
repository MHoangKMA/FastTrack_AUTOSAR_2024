/**
*   @file    PORT_Driver.h
*
*   @version 1.0.0
*
*   @addtogroup PORT
*   @{
*/

#ifndef __PORT_H__
#define __PORT_H__

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
 * @brief Internal pullup/pulldown of each GPIO input pin 
 */
typedef enum PORT_PullSelect_t
{
    portInternalPullDown    = 0U,         /* Internal pull-down resistor is enabled */
    portInternalPullUp      = 1U          /* Internal pull-up resistrr is enabled */
} PORT_PullSelect_type;

/** 
 * @brief Driver Strength mode of each GPIO output pin 
 */
typedef enum PORT_DriveStrengthMode_t
{
    portLowDriverStrength   = 0U,    /* Low driver strength is configured on the corresponding pin */
    portHighDriverStrength  = 1U     /* High driver strength is configured on the corresponding pin */
} PORT_DriveStrengthMode_type;

/** @brief Pin Mux mode of each GPIO pin */
typedef enum PORT_PinMuxSelect_t
{
    portMuxDisable  = 0U,           /* Corresponding pin is disabled, used as an analog pin */
    portMuxAlt1GPIO = 1U,           /* Corresponding pin is configured GPIO */
    portMuxAlt2     = 2U,           /* Chip specific */
    portMuxAlt3     = 3U,           /* Chip specific */
    portMuxAlt4     = 4U,           /* Chip specific */
    portMuxAlt5     = 5U,           /* Chip specific */
    portMuxAlt6     = 6U,           /* Chip specific */
    portMuxAlt7     = 7U            /* Chip specific */
} PORT_PinMuxSelect_type;

/** @brief Interrupt mode of each GPIO input pin */
typedef enum PORT_InterruptConfig_t
{
    portIntDisabled     = 0x0U,     /* Interrupt DMA request is disabled */
    portDmaRisingEdge   = 0x1U,     /* DMA request on rising edge */
    portDmaFallingEdge  = 0x2U,     /* DMA request on falling edge */
    portDmaEitherEdge   = 0x3U,     /* DMA request on either edge */
    portIntLogicZero    = 0x8U,     /* Interrupt when logic zero */
    portIntRisingEdge   = 0x9U,     /* Interrupt on Rising Edge */
    portIntFallingEdge  = 0xAU,     /* Interrupt on Falling Edge */
    portIntEitherEdge   = 0xBU,     /* Interrupt on Either Edge */
    portIntLogicOne     = 0xCU      /* Interrupt when logic one */
} PORT_InterruptConfig_type;

typedef uint8_t PORT_PullEnable_type;
typedef uint8_t PORT_PaFilterEnable_type;
typedef uint8_t PORT_LockRegEnable_type;

/** @brief struct information need to config for a pin */
typedef struct PORT_Config_t
{
    PORT_PullEnable_type isPullEnable;                    /* Enable pullup/pulldown */
    PORT_PullSelect_type pullSelect;                      /* Select pullup/pulldown */
    PORT_PaFilterEnable_type isPassiveFilterEnable;       /* Enable passive filter */
    PORT_DriveStrengthMode_type driverStrengthMode;
    PORT_PinMuxSelect_type muxMode;
    PORT_LockRegEnable_type isLockRegisterEnable;
    PORT_InterruptConfig_type interruptMode;
} PORT_Config_type;

/** @brief struct information need to config for a specific GPIO input pin
*/
typedef struct PORT_PinConfig_t
{
    uint8_t pinCode;                    /* pinCode represent both PORT name and pin number */
    PORT_Config_type userConfig;        /* struct input configure */
} PORT_PinConfig_type;



/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/


/**
 * @brief Initialize a specific GPIO pin
 *
 * @param[in] pinConfig  Pointer to the PORT pin config structure
 *
 * @return void
 */
void PORT_Driver_InitPin(PORT_PinConfig_type *pinConfig);

/**
 * @brief Initialize multiple GPIO pins
 *
 * @param[in] arrayPinsConfig  Pointer to array of PORT pin config structures
 * @param[in] numberPins  Number of pins to be configured
 *
 * @return void
 */
void PORT_Driver_InitMultiplePin(PORT_PinConfig_type *arrayPinsConfig[], int8_t numberPins);

/**
 * @brief Read ISF interrupt flag for individual pin
 *
 * @param[in] pinCode  Pin code represent both PORT name and pin number
 *
 * @return 0 if ISF flag is not set, 1 if ISF flag is set
 */
uint8_t PORT_Driver_ReadInterruptFlagPin(uint8_t pinCode);

/**
 * @brief Clear ISF interrupt flag for individual pin
 *
 * @param[in] pinCode  Pin code represent both PORT name and pin number
 *
 * @return void
 */
void PORT_Driver_ClearInterruptFlagPin(uint8_t pinCode);

/**
 * @brief De-initialize a specific GPIO pin
 *
 * @param[in] pinCode  Pin code represent both PORT name and pin number
 *
 * @return void
 */
void PORT_Driver_DeinitPin(uint8_t pinCode);

/**
 * @brief De-initialize multiple GPIO pins
 *
 * @param[in] arrayPinCode  Pointer to array of pin codes
 * @param[in] numberPins  Number of pins to be de-configured
 *
 * @return void
 */
void PORT_Driver_DeinitMultiplePin(uint8_t *arrayPinCode, uint8_t numberPins);


#ifdef __cplusplus
}
#endif

#endif /* __PORT_H__ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
