

#ifndef _DRV_LPUART_H_
#define _DRV_LPUART_H_
/*================================================================================================
===========================================TYPE DEFINITIONS=======================================
==================================================================================================*/

#include "Driver_Header.h"

typedef enum
{
    DRV_UART_DATABITCOUNT_7 = 0xFFU, /*!< 7-bit data characters */
    DRV_UART_DATABITCOUNT_8 = 0x00U, /*!< 8-bit data characters */
    DRV_UART_DATABITCOUNT_9 = 0x01U, /*!< 9-bit data characters */
    DRV_UART_DATABITCOUNT_10 = 0x02U /*!< 10-bit data characters */
} Drv_Uart_DataBitCountType;

typedef enum
{
    DRV_UART_PARITYMODEDISABLED = 0x00U, /*!< Parity disabled */
    DRV_UART_PARITYMODEEVEN = 0x02U,     /*!< Parity enabled, type even, bit setting: PE|PT = 10 */
    DRV_UART_PARITYMODEODD = 0x03U       /*!< Parity enabled, type odd, bit setting: PE|PT = 11 */
} Drv_Uart_ParityModeType;

typedef enum
{
    DRV_UART_STOPBITCOUNTONE = 0x00U, /*!< One stop bit */
    DRV_UART_STOPBITCOUNTTWO = 0x01U  /*!< Two stop bits */
} Drv_Uart_StopBitCountType;

typedef enum
{
    DRV_UART_NOTUSINGINTERRUPTS = 0x00U, /*!< Not use interrupt to perform UART transfer */
    DRV_UART_USINGINTERRUPTS = 0x01U     /*!< Using interrupts to perform UART transfer */
} Drv_Uart_TransferType;

typedef enum
{
    DRV_UART_CALLBACKERROR = 0x0u,       /* Callback function to handle error */
    DRV_UART_CALLBACKTRANSMITTER = 0x1u, /* Callback function to handle transmitting data */
    DRV_UART_CALLBACKRECEIVER = 0x2u,    /* Callback function to handle receiving data */
} Drv_Uart_CallBackFunctionType;

typedef enum
{
    DRV_UART_BAUDRATEVALUE_600 = 600U,
    DRV_UART_BAUDRATEVALUE_9600 = 9600U,
    DRV_UART_BAUDRATEVALUE_12800 = 12800U,
    DRV_UART_BAUDRATEVALUE_38400 = 38400U,
    DRV_UART_BAUDRATEVALUE_128000 = 128000U,
    DRV_UART_BAUDRATEVALUE_230400 = 230400U,
    DRV_UART_BAUDRATEVALUE_256000 = 256000U,
    DRV_UART_BAUDRATEVALUE_115200 = 115200U,
} Drv_Uart_BaudrateValueType;

typedef enum
{
    DRV_UART_INSTANCE_0 = 0x00U,    /* UART instance 0 */
    DRV_UART_INSTANCE_1 = 0x01U,    /* UART instance 1 */
    DRV_UART_INSTANCE_2 = 0x02U,    /* UART instance 2 */
    DRV_UART_INSTANCECOUNT = 0x03U, /* UART instance count */
} Drv_Uart_InstanceType;

typedef enum
{
    DRV_UART_TXBUSY = 0xFFU,              /*!< Data Transmission process is ongoing> */
    DRV_UART_RXBUSY = 0xFEU,              /*!< Data Reception process is ongoing> */
    DRV_UART_STATEREADY = 0x22U,          /*!< The UART module is free to use> */
    DRV_UART_STATEDEFAULT = 0x55U,        /*!< The UART is in default state. Module is not initialized> */
    DRV_UART_STATEINITIALIZED = 0xEEU,    /*!< The UART module is initialized> */
    DRV_UART_STATERXOVERRUNERROR = 0x01U, /*!< Receiver encountered an overrun error> */
    DRV_UART_STATEFRAMINGERROR = 0x02U,   /*!< Receiver encountered a framing error> */
    DRV_UART_STATEPARITYERROR = 0x03U,    /*!< Receiver encountered a parity error> */
    DRV_UART_STATENOISEERROR = 0x04U,     /*!< Receiver encountered a noise error> */
    DRV_UART_OK = 0x00U,                  /*!< UART module operates OK> */
    DRV_UART_ERROR = 0x05U,               /*!< UART module ERROR> */
    DRV_UART_BUSY = 0x06U,                /*!< UART busy> */
} Drv_Uart_StatusType;

typedef enum
{
    DRV_UART_SOSCCLKSOUCE = 0x00U, /*!< SOSCCLK source = 8000000U Hz> */
    DRV_UART_FIRCCLKSOUCE = 0x01U, /*!< FIRCCLK source = 48000000U Hz> */
} Drv_Uart_ClkSourceType;

/* UART configuration structure */
typedef struct
{
    Drv_Uart_DataBitCountType bitCountPerChar; /*Number of bits in a character*/
    Drv_Uart_ParityModeType parityMode;        /*Parity mode, disabled (default), even, odd*/
    Drv_Uart_StopBitCountType stopBit;         /*Number of stop bits, 1 stop bit (default) or 2 stop bits*/
    Drv_Uart_BaudrateValueType baudRate;       /*UART module baudrate*/
    Drv_Uart_TransferType transferType;        /*UART module transfer type*/
    Drv_Uart_ClkSourceType clockSource;        /*UART module clock source*/
} Drv_Uart_ConfigType;

/* UART receive buffer structure */
typedef struct
{
    uint8_t *prxBuff;       	                        /* pointer points to the receive buffer*/
    uint16_t rxBuffSize; 	                            /* size of the receive buffer*/
    uint16_t rxCount;                                   /* the receive buffer counter*/
    Drv_Uart_StatusType rxStatus;                      /* the status of receiver*/
    bool isRxBusy;                                      /* Check the status of receiver*/

} Drv_Uart_RxBuffType;

/* UART transmit buffer structure */
typedef struct
{
    uint8_t *ptxBuff;                                   /* pointer points to the transmit buffer*/
    uint16_t txBuffSize; 	                            /* size of the receive buffer*/
    uint16_t txCount;                                   /* the transmit buffer counter*/
    Drv_Uart_StatusType txStatus;                       /* the status of transmitter*/
    bool isTxBusy;                                      /* Check the status of transmitter*/
} Drv_Uart_TxBuffType;

/* Function pointer to register the function callback for dectecting errors */
typedef void (*DRV_CallBackErrorLPUART)(Drv_Uart_StatusType error_type);
/* Function pointer to register the function callback */
typedef void (*DRV_CallBack_LPUART)(void);

/*================================================================================================
========================================FUNCTIONS PROTOTYPE=======================================
==================================================================================================*/

/**
 * @brief This function is responsible for initializing UART module
 *
 * @param uartConfig : UART configuration elements: data bits per char, parity mode, stop bit, baud rate, transfer type
 * @return Drv_Uart_StatusType
 */
Drv_Uart_StatusType Drv_Uart_Init(const Drv_Uart_InstanceType instance, const Drv_Uart_ConfigType *uartConfig);

/**
 * @brief This function is responsible for Deinitializing UART module
 *
 * @param instance : instance decides the LPUART base pointer
 * @return Drv_Uart_StatusType
 */
Drv_Uart_StatusType Drv_Uart_Deinit(const Drv_Uart_InstanceType instance);

/**
 * @brief This function is responsible for setting  The UART's baud rate
 *
 * @param instance : instance decides the LPUART base pointer
 * @param baudRate : baud rate for UART module
 * @return Drv_Uart_StatusType
 */
Drv_Uart_StatusType Drv_Uart_SetBaudRate(const Drv_Uart_InstanceType instance, const Drv_Uart_BaudrateValueType baudRate);

/**
 * @brief This function is responsible for transmitting data via interrupt method
 *
 * @param instance : instance decides the LPUART base pointer
 * @param txBuffer : transmit buffer
 * @param length   : transmit buffer size
 * @return Drv_Uart_StatusType
 */
Drv_Uart_StatusType Drv_Uart_SendDataInterrupt(const Drv_Uart_InstanceType instance, uint8_t *data, uint16_t length);

/**
 * @brief This function is responsible for receiving data via interrupt method
 *
 * @param instance : instance decides the LPUART base pointer
 * @param data     : receive buffer
 * @param length   : receive buffer size
 * @return Drv_Uart_StatusType
 */
Drv_Uart_StatusType Drv_Uart_ReceiveDataInterrupt(const Drv_Uart_InstanceType instance, const uint8_t *data, uint16_t length);

/**
 * @brief This function is responsible for aborting the receiver
 *
 * @param instance : instance decides the LPUART base pointer
 */
Drv_Uart_StatusType Drv_Uart_AbortReceiving(const Drv_Uart_InstanceType instance);

/**
 * @brief This function is responsible for aborting the transmitter
 *
 * @param instance : instance decides the LPUART base pointer
 * @return Drv_Uart_StatusType
 */
Drv_Uart_StatusType Drv_Uart_AbortTransmitting(const Drv_Uart_InstanceType instance);

/**
 * @brief : This function is responsible for registering function callback depending on the callback function type
 *
 * @param callBackType : Choose type of the callback function
 * @param cbFunction : The pointer points to a function that be called when an interrupt happens
 */
void Drv_Uart_InstallCallBack(Drv_Uart_CallBackFunctionType callBackType, DRV_CallBack_LPUART cbFunction);

/**
 * @brief This function is responsible for registering error detecting function
 *
 * @param cbFunctionE : The pointer points to a function that be called when an errors via UART interrupt happens
 */
void Drv_Uart_InstallCallBackE(DRV_CallBackErrorLPUART cbFunctionE);

/**
 * @brief This function is responsible for disabling the Tx
 *
 * @param instance : instance decides the LPUART base pointer
 */
void Drv_Uart_DisableTx(const Drv_Uart_InstanceType instance);

/**
 * @brief This function is responsible for disabling the Rx
 *
 * @param instance : instance decides the LPUART base pointer
 */
void Drv_Uart_DisableRx(const Drv_Uart_InstanceType instance);

/**
 * @brief : This function is responsible for enabling the Tx
 *
 */
void Drv_Uart_EnableTx(const Drv_Uart_InstanceType instance);

/**
 * @brief : This function is responsible for enabling the Rx
 *
 */
void Drv_Uart_EnableRx(const Drv_Uart_InstanceType instance);

#endif /* _DRV_LPUART_H_ */
