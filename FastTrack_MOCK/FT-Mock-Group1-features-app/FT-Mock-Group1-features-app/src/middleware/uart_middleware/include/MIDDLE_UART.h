#ifndef INC_MIDD_UART_H_
#define INC_MIDD_UART_H_

#include "Driver_Header.h"

/*==================================================================================================
*                                        ENUMS
==================================================================================================*/

/**
 * @brief Enumeration of UART callback functions.
 *
 * Defines the types of callback functions that can be triggered in the UART middleware.
 */
typedef enum
{
    MID_UART_callBackError         = 0x0u,  /*!< Callback function to handle UART error events */
    MID_UART_callBackTransmitter   = 0x1u,  /*!< Callback function to handle data transmission events */
    MID_UART_callBackReceiver      = 0x2u,  /*!< Callback function to handle data reception events */
} MID_UART_CallBackFunctionType;

/**
 * @brief Enumeration of UART instances.
 *
 * Defines the available UART hardware instances on the MCU.
 */
typedef enum
{
    MID_UART_instance_0 = 0x00U,  /*!< UART instance 0 */
    MID_UART_instance_1 = 0x01U,  /*!< UART instance 1 */
    MID_UART_instance_2 = 0x02U,  /*!< UART instance 2 */
    MID_UART_instanceCount = 0x03U, /*!< Total number of UART instances */
} MID_UART_InstanceType;

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

/**
 * @brief Initializes the UART middleware.
 *
 * This function configures the UART hardware and prepares it for communication.
 */
void MID_UART_Init(void);

/**
 * @brief Runs the application logic for the UART middleware.
 *
 * This function handles the main processing loop or setup for UART-based operations.
 */
void MIDD_AppRun(void);

/**
 * @brief Installs a callback function for a specific UART event.
 *
 * This function allows the user to set callback functions to handle UART events, such as errors, transmission, or reception.
 *
 * @param[in] callBackType  Type of the callback (e.g., error, transmitter, receiver).
 * @param[in] cbFunction    The callback function to handle the specific event.
 */
void MID_UART_InstallCallBack(MID_UART_CallBackFunctionType callBackType, DRV_CallBack_LPUART cbFunction);

/**
 * @brief Receives data via UART interrupt.
 *
 * This function is called when UART interrupt occurs to handle incoming data.
 *
 * @param[in] instance  The UART instance from which data is received.
 * @param[in] rxBuff    Pointer to the buffer where received data will be stored.
 * @param[in] rxSize    The size of the received data buffer.
 */
void MID_UART_ReceiveDataInterrupt(const MID_UART_InstanceType instance, const uint8_t *rxBuff, const uint16_t rxSize);

/**
 * @brief Sends data via UART interrupt.
 *
 * This function is called to initiate data transmission via UART interrupt.
 *
 * @param[in] instance  The UART instance used for sending data.
 * @param[in] data      Pointer to the buffer containing data to be sent.
 * @param[in] length    The number of bytes to transmit.
 */
void MID_UART_SendDataInterrupt(const MID_UART_InstanceType instance, uint8_t *data, uint16_t length);

#endif /* INC_MIDD_UART_H_ */

