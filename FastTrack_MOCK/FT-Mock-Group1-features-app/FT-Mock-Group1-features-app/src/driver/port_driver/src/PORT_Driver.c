/**
*   @file    PORT_Driver.c
*
*   @version 1.0.0
*
*   @addtogroup PORT
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

#include "PORT_Driver.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

#define PORT_NUMBER_OF_PINS             17
#define PORT_EXTRACT_INSTANCE(value)    (uint8_t)(((value) & 0xE0) >> 5)
#define PORT_EXTRACT_PIN(value)         (uint8_t)((value) & 0x1F)

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

PORT_Type *g_portBase[PORT_MAX_INSTANCE] = IP_PORT_BASE_PTRS;

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/


void PORT_Driver_InitPin(PORT_PinConfig_type *pinConfig)
{
    uint8_t pin = PORT_EXTRACT_PIN(pinConfig->pinCode);
    uint8_t port_instance = PORT_EXTRACT_INSTANCE(pinConfig->pinCode);
    PORT_Type *base = g_portBase[port_instance];

//    assert(pin < PORT_NUMBER_OF_PINS);
//    assert(port_instance < PORT_MAX_INSTANCE);
    if ((void *)0 != pinConfig)
    {
        /* Pullup/pulldown configure */
        if (pinConfig->userConfig.isPullEnable)
        {
            base->PCR[pin] |= PORT_PCR_PE_MASK;
            if (portInternalPullDown == pinConfig->userConfig.pullSelect)
            {
                base->PCR[pin] &= PORT_PCR_PS_MASK;
            }
            else
            {
                base->PCR[pin] |= PORT_PCR_PS_MASK;
            }
        }
        /* Passive filter configure */
        if (pinConfig->userConfig.isPassiveFilterEnable)
        {
            base->PCR[pin] |= PORT_PCR_PFE_MASK;
        }
        /* Driver strength mode */
        if (portLowDriverStrength == pinConfig->userConfig.driverStrengthMode)
        {
            base->PCR[pin] &= PORT_PCR_DSE_MASK;
        }
        else
        {
            base->PCR[pin] |= PORT_PCR_DSE_MASK;
        }
        /* Pin muxing control */
        if (portMuxDisable != pinConfig->userConfig.muxMode)
        {
            base->PCR[pin] &= PORT_PCR_MUX_MASK;
            base->PCR[pin] |= PORT_PCR_MUX((uint8_t)pinConfig->userConfig.muxMode);
        }
        /* Lock register */
        if(pinConfig->userConfig.isLockRegisterEnable)
        {
            base->PCR[pin] |= PORT_PCR_LK_MASK;
        }
        /* Interrupt configure */
        if (portIntDisabled != pinConfig->userConfig.interruptMode)
        {
            base->PCR[pin] &= PORT_PCR_IRQC_MASK;
            base->PCR[pin] |= PORT_PCR_IRQC((uint8_t)pinConfig->userConfig.interruptMode);
        }
    }
}

void PORT_Driver_InitMultiplePin(PORT_PinConfig_type *arrayPinsConfig[], int8_t numberPins)
{
    if (((void *)0 != arrayPinsConfig) && (0 != numberPins))
    {
        while(numberPins)
        {
            PORT_Driver_InitPin(arrayPinsConfig[numberPins - 1]);
            numberPins--;
        }
    }
}

uint8_t PORT_Driver_ReadInterruptFlagPin(uint8_t pinCode)
{
    uint8_t pin = PORT_EXTRACT_PIN(pinCode);
    uint8_t port_instance = PORT_EXTRACT_INSTANCE(pinCode);
    PORT_Type *base = g_portBase[port_instance];

    assert(pin < PORT_NUMBER_OF_PINS);
    assert(port_instance < PORT_MAX_INSTANCE);
    return (uint8_t)((base->PCR[pin] & PORT_PCR_ISF_MASK) >> PORT_PCR_ISF_SHIFT);
}

void PORT_Driver_ClearInterruptFlagPin(uint8_t pinCode)
{
    uint8_t pin = PORT_EXTRACT_PIN(pinCode);
    uint8_t port_instance = PORT_EXTRACT_INSTANCE(pinCode);
    PORT_Type *base = g_portBase[port_instance];

    assert(pin < PORT_NUMBER_OF_PINS);
    assert(port_instance < PORT_MAX_INSTANCE);
    base->PCR[pin] |= (1 << PORT_PCR_ISF_SHIFT);
}

void PORT_Driver_DeinitPin(uint8_t pinCode)
{
    uint8_t pin = PORT_EXTRACT_PIN(pinCode);
    uint8_t port_instance = PORT_EXTRACT_INSTANCE(pinCode);
    PORT_Type *base = g_portBase[port_instance];

    assert(pin < PORT_NUMBER_OF_PINS);
    assert(port_instance < PORT_MAX_INSTANCE);
    /* Avoid write into reserved bit */
    base->PCR[pin] &= PORT_PCR_PS_MASK;
    base->PCR[pin] &= PORT_PCR_PE_MASK;
    base->PCR[pin] &= PORT_PCR_PFE_MASK;
    base->PCR[pin] &= PORT_PCR_DSE_MASK;
    base->PCR[pin] &= PORT_PCR_MUX_MASK;
    base->PCR[pin] &= PORT_PCR_LK_MASK;
    base->PCR[pin] &= PORT_PCR_IRQC_MASK;
    /* Write 1 to clear */
    base->PCR[pin] |= PORT_PCR_ISF_MASK;
}

void PORT_Driver_DeinitMultiplePin(uint8_t *arrayPinCode, uint8_t numberPins)
{
    if (((void *)0 != arrayPinCode) && (0 != numberPins))
    {
        while(numberPins)
        {
            PORT_Driver_DeinitPin(arrayPinCode[numberPins - 1]);
            numberPins--;
        }
    }
}

#ifdef __cplusplus
}
#endif
