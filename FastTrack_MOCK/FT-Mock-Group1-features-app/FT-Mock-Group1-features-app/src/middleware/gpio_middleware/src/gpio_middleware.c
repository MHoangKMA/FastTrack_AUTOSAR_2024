/*
 * Mid_Lpit.c
 *
 *  Created on: Nov 7, 2024
 *      Author: HI
 */
/*******************************************************************************
 * Include
 *******************************************************************************/
#include "../src/middleware/gpio_middleware/include/gpio_middleware.h"
/*******************************************************************************
 * Variables
 *******************************************************************************/

/*******************************************************************************
 * Api
 *******************************************************************************/

/*******************************************************************************
 * Code
 *******************************************************************************/
void MID_GPIO_LEDInit(LED_Color_t Color)
{
    PORT_Config_type PORTConfig = {
        .muxMode = portMuxAlt1GPIO
    };
    PORT_PinConfig_type PORT_Cfg = {
        .pinCode = Color,
        .userConfig = PORTConfig};
    PORT_Driver_InitPin(&PORT_Cfg);
    GPIO_PinConfig_type Cfg = {Color, GPIO_PIN_MODE_OUTPUT};
    GPIO_Driver_ModeConfig(&Cfg);
}

void MID_GPIO_Init()
{
    PCC_PeriClockControl(PCC_PORTD_INDEX, CLOCK_NOSRC_CLK, CLOCK_DIV_DISABLED, ENABLE);
    MID_GPIO_LEDInit(GREEN);
    MID_GPIO_LEDInit(RED);
    MID_GPIO_LEDInit(BLUE);
}

void MID_GPIO_LEDOff(LED_Color_t Color)
{
    GPIO_Driver_SetOutput(Color);
}

void MID_GPIO_LEDOn(LED_Color_t Color)
{
    GPIO_Driver_ClearOutput(Color);
}
/*******************************************************************************
 * end of file
 *******************************************************************************/
