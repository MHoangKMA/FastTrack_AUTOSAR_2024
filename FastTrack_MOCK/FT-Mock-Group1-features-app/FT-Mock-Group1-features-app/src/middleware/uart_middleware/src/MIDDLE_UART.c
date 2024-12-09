#include "MIDDLE_UART.h"

/************************* Macro *********************************/
#define UART_TX_PIN 71U
#define UART_RX_PIN 70U

/*********************** Static function prototypes ****************/
static void MIDD_uartInit(void);
static void MIDD_clockInit(void);

/*********************** Configuration Variables *******************/
Drv_Uart_ConfigType UserConfig = {
    .baudRate = DRV_UART_BAUDRATEVALUE_115200,
    .bitCountPerChar = DRV_UART_DATABITCOUNT_8,
    .clockSource = DRV_UART_FIRCCLKSOUCE,
    .parityMode = DRV_UART_PARITYMODEDISABLED,
    .stopBit = DRV_UART_STOPBITCOUNTONE,
    .transferType = DRV_UART_USINGINTERRUPTS};

PORT_Config_type PORTConfig = {
  .muxMode = portMuxAlt2,
};

/*************************** Functions *****************************/

static void MIDD_clockInit(void) {
    // Enable clock for PORTC using the desired clock source and divider settings
    PCC_PeriClockControl(PCC_PORTC_INDEX, CLOCK_SIRCDIV2_CLK, CLOCK_DIV_1, ENABLE);

    // Enable clock for LPUART1 with appropriate source and divider settings
    PCC_PeriClockControl(PCC_LPUART1_INDEX, CLOCK_FIRCDIV2_CLK, CLOCK_DIV_1, ENABLE);
}


static void MIDD_uartInit(void) {
  /* Initialize Tx and Rx pin configurations */

  PORT_PinConfig_type s_txPinCf = {
    .pinCode = UART_TX_PIN,
    .userConfig = PORTConfig
  };

  PORT_PinConfig_type s_rxPinCf = {
    .pinCode = UART_RX_PIN,
    .userConfig = PORTConfig
  };

  /* Initialize Tx and Rx pins */
  PORT_Driver_InitPin(&s_txPinCf);
  PORT_Driver_InitPin(&s_rxPinCf);

  /* Initialize UART with the specified configuration */
  Drv_Uart_Init(DRV_UART_INSTANCE_1, &UserConfig);
  NVIC_EnableIRQn(LPUART1_RxTx_IRQn);

}

void MID_UART_Init(void) {
  /* Initialize clock and UART */
  MIDD_clockInit();
  MIDD_uartInit();
}

void MID_UART_InstallCallBack(MID_UART_CallBackFunctionType callBackType, DRV_CallBack_LPUART cbFunction)
{
	Drv_Uart_InstallCallBack(callBackType, cbFunction);
}

void MID_UART_ReceiveDataInterrupt(const MID_UART_InstanceType instance, const uint8_t *rxBuff, const uint16_t rxSize)
{
  Drv_Uart_ReceiveDataInterrupt(instance, rxBuff, rxSize);
}

void MID_UART_SendDataInterrupt(const MID_UART_InstanceType instance, uint8_t *data, uint16_t length)
{
  Drv_Uart_SendDataInterrupt(instance, data, length);
}


