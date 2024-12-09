/*
 * node_temp.h
 *
 *  Created on: Nov 18, 2024
 *      Author: Manh
 */

#ifndef APP_NODE_TEMPERATURE_INCLUDE_NODE_TEMP_H_
#define APP_NODE_TEMPERATURE_INCLUDE_NODE_TEMP_H_

#include "Middleware_Header.h"

/* Enumeration for temperature ping state */
/**
 * @brief Enumeration for the temperature ping state.
 *
 * Defines the possible states for the temperature ping readiness.
 */
typedef enum {
    TEMP_PING_READY,         /*!< Temperature ping is ready */
    TEMP_PING_NOT_READY      /*!< Temperature ping is not ready */
} Temp_Ping_State_t;

/* Enumeration for temperature connection state */
/**
 * @brief Enumeration for temperature connection state.
 *
 * Defines the possible states of the temperature node connection.
 */
typedef enum {
    TEMP_OK = 0u,            /*!< Temperature node is successfully connected */
    TEMP_NOT_OK = 1u         /*!< Temperature node is not connected */
} Temp_Connect_State_t;

/*****************************************************************************/
/* Public Function Prototypes                                                */
/*****************************************************************************/

/**
 * @brief The main function to manage temperature node operations.
 *
 * This function handles the initialization, communication, and data processing
 * for the temperature node. It ensures the proper reception and processing of temperature
 * data and manages related operations based on system states.
 *
 * @param None
 * @return None
 */
void App_NodeTemp_Run();

#endif /* APP_NODE_TEMPERATURE_INCLUDE_NODE_TEMP_H_ */
