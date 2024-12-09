/*
 * NVIC.h
 *
 *  Created on: Apr 24, 2024
 *      Author: Le Hung
 */

#ifndef INC_NVIC_H_
#define INC_NVIC_H_

#include "Driver_Header.h"

/* ----------------------------------------------------------------------------
   -- Definitions
   ---------------------------------------------------------------------------- */
/**
 * @brief Nested Vectored Interrupt Controller (NVIC) structure.
 *
 * This structure represents the NVIC (Nested Vectored Interrupt Controller) registers.
 * The NVIC is responsible for managing interrupts in ARM Cortex-M processors.
 */
typedef struct {
    uint32_t ISER[8];      			/**< NVIC Interrupt Set-enable Registers */
    uint32_t RESERVED_1[24]; 		/**< Padding to align with next register block */
    uint32_t ICER[8];      			/**< NVIC Interrupt Clear-enable Registers */
    uint32_t RESERVED_2[24]; 		/**< Padding */
    uint32_t ISPR[8];      			/**< NVIC Interrupt Set-pending Registers */
    uint32_t RESERVED_3[24]; 		/**< Padding */
    uint32_t ICPR[8];      			/**< NVIC Interrupt Clear-pending Registers */
    uint32_t RESERVED_4[24]; 		/**< Padding */
    uint32_t IABR[8];      			/**< NVIC Interrupt Active Bit Registers */
    uint32_t RESERVED_5[56]; 		/**< Padding */
    uint32_t IPR[60];      			/**< NVIC Interrupt Priority Registers */
    uint32_t RESERVED_6[644]; 		/**< Padding */
    uint32_t STIR;         			/**< NVIC Software Trigger Interrupt Register (Write-only) */
} NVIC_Type;

/* NVIC base address */
#define NVIC_BASE				(0xE000E100u)
/* NVIC base pointer */
#define NVIC					((NVIC_Type*)NVIC_BASE)

#define NVIC_PR_BASEADDR        ((volatile uint32_t*)0xE000E400u)

/* ----------------------------------------------------------------------------
   -- API
   ---------------------------------------------------------------------------- */

/**
 * @brief Enables a specific interrupt in the Nested Vectored Interrupt Controller (NVIC).
 *
 * This function enables the specified interrupt in the NVIC, allowing it to generate interrupts.
 *
 * @param IRQn The IRQ number (enum value) of the interrupt to enable.
 */
void NVIC_EnableIRQn(IRQn_Type IRQn);

/**
 * @brief Disables a specific interrupt in the Nested Vectored Interrupt Controller (NVIC).
 *
 * This function disables the specified interrupt in the NVIC, preventing it from generating interrupts.
 *
 * @param IRQn The IRQ number (enum value) of the interrupt to disable.
 */
void NVIC_DisableIRQn(IRQn_Type IRQn);

void NVIC_IRQPriorityConfig(uint8_t IRQ_number, uint32_t IRQ_Priority);

#endif /* INC_NVIC_H_ */
