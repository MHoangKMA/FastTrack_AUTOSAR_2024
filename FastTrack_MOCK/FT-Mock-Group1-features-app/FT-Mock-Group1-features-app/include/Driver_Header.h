/*
 * Driver_Header.h
 *
 *  Created on: Nov 12, 2024
 *      Author: Manh
 */

#ifndef DRIVER_DRIVER_HEADER_H_
#define DRIVER_DRIVER_HEADER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "S32K144.h"
#include "../src/driver/adc_driver/include/ADC_Driver.h"
#include "../src/driver/can_driver/include/FlexCAN.h"
#include "../src/driver/clock_driver/include/CLOCK.h"
#include "../src/driver/gpio_driver/include/GPIO_Driver.h"
#include "../src/driver/NVIC/include/NVIC.h"
#include "../src/driver/port_driver/include/PORT_Driver.h"
#include "../src/driver/lpit_driver/include/Drv_Lpit.h"
#include "../src/driver/uart_driver/include/DRV_LPUART.h"
#include "../src/driver/adc_driver/include/ADC_Driver.h"
#include "../src/type_common/type_common.h"
#include "assert.h"


#define ENABLE 1u
#define DISABLE 0u

#define SET 1u

#endif /* DRIVER_DRIVER_HEADER_H_ */
