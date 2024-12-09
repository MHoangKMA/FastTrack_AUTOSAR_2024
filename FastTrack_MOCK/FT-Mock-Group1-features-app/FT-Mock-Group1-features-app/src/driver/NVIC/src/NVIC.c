/*
 * NVIC.c
 *
 *  Created on: Apr 24, 2024
 *      Author: Le Hung
 */

#include "NVIC.h"

/**
 * @brief Enables a specific interrupt in the Nested Vectored Interrupt Controller (NVIC).
 */
void NVIC_EnableIRQn(IRQn_Type IRQn)
{
	uint8_t  n = 0; /* ISER[n] */
	uint8_t bitPos = 0; /* Bit position of the IRQnumber in the corresponding ISER[n]*/

	/* Find the corresponding Register of IRQnumber*/
	n = IRQn / 32;

	/* Find the bit position*/
	bitPos = IRQn % 32;

	/* Enable IRQ Line*/
	NVIC->ISER[n] |= (1 << bitPos);
}

/**
 * @brief Disables a specific interrupt in the Nested Vectored Interrupt Controller (NVIC).
 */
void NVIC_DisableIRQn(IRQn_Type IRQn)
{
	uint8_t n = 0; 		/* ISER[n] */
	uint8_t bitPos = 0; /* Bit position of the IRQnumber in the corresponding ISER[n]*/

	/* Find the corresponding Register of IRQnumber*/
	n = IRQn / 32;

	/* Find the bit position*/
	bitPos = IRQn % 32;

	/* Disable IRQ Line*/
	NVIC->ICER[n] |= (1 << bitPos);
}

void NVIC_IRQPriorityConfig(uint8_t IRQ_number, uint32_t IRQ_Priority)
{
	/* Find NVIC_IPRx and section of IQRx_PRI */
	uint8_t IPRx = IRQ_number/4;
	uint8_t IRQx_Section = IRQ_number % 4;

	/* Cause 4 bits LSB of 1 section are reserved => Only need to modify 4 bits MSB of section to set Priority for IRQ_Numbers */
	uint8_t shift_amount = (8 * IRQx_Section) + 4;

	/* Modify NVIC_PRx to set Priority */
	*(NVIC_PR_BASEADDR + IPRx) |= (IRQ_Priority << shift_amount);
}

/* ----------------------------------------------------------------------------
   -- End of File
   ---------------------------------------------------------------------------- */
