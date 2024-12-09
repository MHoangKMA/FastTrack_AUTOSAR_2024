/*****************************************************************************/
/* Header Guard                                                              */
/*****************************************************************************/
/* Prevent multiple inclusions of the header file.                           */

#ifndef NODE_SPEED_H_
#define NODE_SPEED_H_

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/
/* Include necessary middleware and driver headers for functionality.        */

#include "Middleware_Header.h"
#include "Driver_Header.h"

/*****************************************************************************/
/* Enumerations                                                              */
/*****************************************************************************/
/* Define enumerations for speed ping states and connection states.          */

/**
 * @brief Enumeration for the speed ping state.
 *
 * Defines the possible states of the speed ping readiness.
 */
typedef enum {
    SPEED_PING_READY,         /*!< Indicates that speed ping is ready */
    SPEED_PING_NOT_READY,     /*!< Indicates that speed ping is not ready */
} Speed_Ping_State_t;

/**
 * @brief Enumeration for speed connection state.
 *
 * Defines whether the speed connection is established or failed.
 */
typedef enum {
    SPEED_OK = 0u,            /*!< Indicates a successful speed connection */
    SPEED_NOT_OK = 1u,        /*!< Indicates a failed speed connection */
} Speed_Connect_State_t;

/*****************************************************************************/
/* Public Function Prototypes                                                */
/*****************************************************************************/

/**
 * @brief The main function to manage speed node operations.
 *
 * This function handles the initialization, communication, and data processing
 * for the speed node. It ensures the proper reception and processing of speed
 * data and manages related operations based on system states.
 *
 * @param None
 * @return None
 */
void App_NodeSpeed_Run(void);

#endif /* NODE_SPEED_H_ */
