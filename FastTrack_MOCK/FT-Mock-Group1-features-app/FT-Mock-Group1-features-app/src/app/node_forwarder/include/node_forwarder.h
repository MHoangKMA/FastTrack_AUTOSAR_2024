/*****************************************************************************/
/* Header Guard                                                              */
/*****************************************************************************/
/* Prevent multiple inclusions of the header file.                           */

#ifndef NODE_FORWARDER_H_
#define NODE_FORWARDER_H_

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/
/* Include necessary middleware and driver headers for functionality.        */

#include "Middleware_Header.h"
#include "Driver_Header.h"

/*****************************************************************************/
/* Enumerations                                                              */
/*****************************************************************************/
/* Define enumerations for CAN states, error states, and connection states.  */

/**
 * @brief Enumeration for CAN speed states.
 *
 * Defines the possible states of the CAN speed readiness.
 */
typedef enum {
    CAN_SPEED_READY = 0u,     /*!< CAN speed is ready */
    CAN_SPEED_NOT_READY,      /*!< CAN speed is not ready */
    CAN_TEMP_READY,           /*!< CAN temperature is ready */
    CAN_TEMP_NOT_READY,       /*!< CAN temperature is not ready */
} CAN_State_t;

/**
 * @brief Enumeration for temperature error states.
 *
 * Defines the possible states of temperature error.
 */
typedef enum {
    TEMP_ERROR = 0u,         /*!< There is a temperature error */
    TEMP_NOT_ERROR = 1u,     /*!< No temperature error */
    TEMP_STILL_ERROR,        /*!< Temperature error persists */
} TEMP_ERROR_State_t;

/**
 * @brief Enumeration for speed error states.
 *
 * Defines the possible states of speed error.
 */
typedef enum {
    SPEED_ERROR = 0u,        /*!< There is a speed error */
    SPEED_NOT_ERROR = 1u,    /*!< No speed error */
    SPEED_STILL_ERROR,       /*!< Speed error persists */
} SPEED_ERROR_State_t;

/**
 * @brief Enumeration for forwarder connection states.
 *
 * Defines whether the forwarder connection is established or not.
 */
typedef enum {
    FWD_OK = 0u,             /*!< Forwarder connection is established */
    FWD_NOT_OK = 1u,         /*!< Forwarder connection failed */
} FWD_Connect_State_t;

/*****************************************************************************/
/* Structures                                                                */
/*****************************************************************************/
/* Define a data structure for storing speed and temperature data.           */

/**
 * @brief Data structure to store speed and temperature data of the node.
 *
 * This structure holds the speed and temperature data that will be used for processing.
 */
typedef struct {
    uint8_t NODE_Speed_Data;   /*!< The speed data of the node */
    uint8_t NODE_Temp_Data;    /*!< The temperature data of the node */
} Data_t;

/*****************************************************************************/
/* Function Prototypes                                                       */
/*****************************************************************************/

/**
 * @brief The main function for running the forwarder application.
 *
 * This function initializes all necessary peripherals, including GPIO, UART,
 * CAN, and timers. It handles the reception and processing of data from CAN
 * and UART, manages LED indicators based on threshold conditions, and ensures
 * proper system operation through a continuous processing loop.
 *
 * @param None
 * @return None
 */
void App_Forwarder_Run(void);

#endif /* NODE_FORWARDER_H_ */
