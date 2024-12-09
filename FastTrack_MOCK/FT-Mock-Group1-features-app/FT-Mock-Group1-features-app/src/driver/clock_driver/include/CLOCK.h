/*
 * SCG.h
 *
 *  Created on: Oct 31, 2024
 *      Author: adm
 */

#ifndef INCLUDE_CLOCK_H_
#define INCLUDE_CLOCK_H_

#include "S32K144.h"

/* ----------------------------------------------------------------------------
   -- Definitions
   ---------------------------------------------------------------------------- */

/**
 * @brief Enum to define the different clock sources for SCG (System Clock Generator).
 */
typedef enum
{
	SCG_SCS_SOSC				= 1U, /*!< External system oscillator clock */
	SCG_SCS_SIRC				= 2U, /*!< Slow internal reference clock */
	SCG_SCS_FIRC				= 3U, /*!< Fast internal reference clock */
	SCG_SCS_SPLL				= 6U  /*!< System PLL clock */
}SCG_SCSType_e;

/**
 * @brief Enum to define the divider values for the core clock in SCG.
 */
typedef enum
{
	SCG_DIVCORE_1				= 0U, /*!< Core clock division by 1 */
	SCG_DIVCORE_2,               /*!< Core clock division by 2 */
	SCG_DIVCORE_3,               /*!< Core clock division by 3 */
	SCG_DIVCORE_4,               /*!< Core clock division by 4 */
	SCG_DIVCORE_5,               /*!< Core clock division by 5 */
	SCG_DIVCORE_6,               /*!< Core clock division by 6 */
	SCG_DIVCORE_7,               /*!< Core clock division by 7 */
	SCG_DIVCORE_8,               /*!< Core clock division by 8 */
	SCG_DIVCORE_9,               /*!< Core clock division by 9 */
	SCG_DIVCORE_10,              /*!< Core clock division by 10 */
	SCG_DIVCORE_11,              /*!< Core clock division by 11 */
	SCG_DIVCORE_12,              /*!< Core clock division by 12 */
	SCG_DIVCORE_13,              /*!< Core clock division by 13 */
	SCG_DIVCORE_14,              /*!< Core clock division by 14 */
	SCG_DIVCORE_15,              /*!< Core clock division by 15 */
	SCG_DIVCORE_16               /*!< Core clock division by 16 */
}SCG_DIVCOREType_e;

/**
 * @brief Enum to define the divider values for the bus clock in SCG.
 */
typedef enum
{
	SCG_DIVBUS_1				= 0U, /*!< Bus clock division by 1 */
	SCG_DIVBUS_2,               /*!< Bus clock division by 2 */
	SCG_DIVBUS_3,               /*!< Bus clock division by 3 */
	SCG_DIVBUS_4,               /*!< Bus clock division by 4 */
	SCG_DIVBUS_5,               /*!< Bus clock division by 5 */
	SCG_DIVBUS_6,               /*!< Bus clock division by 6 */
	SCG_DIVBUS_7,               /*!< Bus clock division by 7 */
	SCG_DIVBUS_8,               /*!< Bus clock division by 8 */
	SCG_DIVBUS_9,               /*!< Bus clock division by 9 */
	SCG_DIVBUS_10,              /*!< Bus clock division by 10 */
	SCG_DIVBUS_11,              /*!< Bus clock division by 11 */
	SCG_DIVBUS_12,              /*!< Bus clock division by 12 */
	SCG_DIVBUS_13,              /*!< Bus clock division by 13 */
	SCG_DIVBUS_14,              /*!< Bus clock division by 14 */
	SCG_DIVBUS_15,              /*!< Bus clock division by 15 */
	SCG_DIVBUS_16               /*!< Bus clock division by 16 */
}SCG_DIVBUSType_e;

/**
 * @brief Enum to define the divider values for the slow clock in SCG.
 */
typedef enum
{
	SCG_DIVSLOW_1				= 0U, /*!< Slow clock division by 1 */
	SCG_DIVSLOW_2,              /*!< Slow clock division by 2 */
	SCG_DIVSLOW_3,              /*!< Slow clock division by 3 */
	SCG_DIVSLOW_4,              /*!< Slow clock division by 4 */
	SCG_DIVSLOW_5,              /*!< Slow clock division by 5 */
	SCG_DIVSLOW_6,              /*!< Slow clock division by 6 */
	SCG_DIVSLOW_7,              /*!< Slow clock division by 7 */
	SCG_DIVSLOW_8               /*!< Slow clock division by 8 */
}SCG_DIVSLOWType_e;

/**
 * @brief Enum to define the available system frequencies in SCG.
 */
typedef enum
{
	SCG_SYS_FREQ_SLOWRUN	= 48000000U,  /*!< System frequency in slow run mode (48 MHz) */
	SCG_SYS_FREQ_NORMALRUN	= 80000000U,  /*!< System frequency in normal run mode (80 MHz) */
	SCG_SYS_FREQ_HSRUN		= 112000000U, /*!< System frequency in high-speed run mode (112 MHz) */
	SCG_SYS_FREQ_HSRUN80	= 80000000U,  /*!< System frequency in high-speed run mode at 80 MHz */
	SCG_SYS_FREQ_VLPRUN	= 4000000U    /*!< System frequency in very low power run mode (4 MHz) */
}SCG_SysFreqType_e;

/**
 * @brief Enum to define peripheral clock source options.
 */
typedef enum
{
	CLOCK_NOSRC_CLK			= 0U, /*!< No clock source */
	CLOCK_SOSCDIV2_CLK		= 1U, /*!< SOSC divided by 2 clock source */
	CLOCK_SIRCDIV2_CLK		= 2U, /*!< SIRC divided by 2 clock source */
	CLOCK_FIRCDIV2_CLK		= 3U, /*!< FIRC divided by 2 clock source */
	CLOCK_SPLLDIV2_CLK		= 6U  /*!< SPLL divided by 2 clock source */
}Clock_PeriClockSrc_e;

/**
 * @brief Enum to define clock dividers for different peripheral clocks.
 */
typedef enum
{
	CLOCK_DIV_DISABLED		= 0U, /*!< Clock division disabled */
	CLOCK_DIV_1,              /*!< Clock division by 1 */
	CLOCK_DIV_2,              /*!< Clock division by 2 */
	CLOCK_DIV_4,              /*!< Clock division by 4 */
	CLOCK_DIV_8,              /*!< Clock division by 8 */
	CLOCK_DIV_16,             /*!< Clock division by 16 */
	CLOCK_DIV_32,             /*!< Clock division by 32 */
	CLOCK_DIV_64              /*!< Clock division by 64 */
}Clock_ClkDiv_e;

/**
 * @brief Structure to hold SCG mode configuration elements.
 */
typedef struct
{
	SCG_SCSType_e 		ScsValue;      /*!< SCG Clock source */
	SCG_DIVCOREType_e	DivCoreValue;  /*!< Core clock divider */
	SCG_DIVBUSType_e	DivBusValue;   /*!< Bus clock divider */
	SCG_DIVSLOWType_e	DivSlowValue;  /*!< Slow clock divider */
}SCG_ModeElementType;

/**
 * @brief Control the peripheral clock of a specific module.
 *
 * This function configures the clock source, divider, and enables/disables the clock for a given peripheral.
 *
 * @param PCCIndex: Index of the module to control the clock
 * @param ClkSrc: Clock source for the module
 * @param DivVal: Divider value for the module's clock
 * @param EnOrDis: Enable or disable the peripheral clock
 */
void PCC_PeriClockControl(uint8_t PCCIndex, Clock_PeriClockSrc_e ClkSrc, Clock_ClkDiv_e DivVal, uint8_t EnOrDis);

/**
 * @brief Get the current system frequency.
 *
 * This function retrieves the current system frequency based on the SCG configuration.
 *
 * @return The current system frequency
 */
SCG_SysFreqType_e SCG_GetSysFreq(void);

#endif /* INCLUDE_CLOCK_H_ */
