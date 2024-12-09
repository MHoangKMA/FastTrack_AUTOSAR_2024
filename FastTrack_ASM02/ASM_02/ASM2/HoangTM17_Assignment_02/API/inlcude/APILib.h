 /*
 * filename: APIsrc.h
 *
 *  Created on: Sep 05, 2024
 *      Author: Hoang Tran-Minh 
 *      Mail: HoangTM17@fpt.com
 */

#ifndef _AUTOTESTLIB_H_
#define _AUTOTESTLIB_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*******************************************************************************
 * TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 ******************************************************************************/
typedef enum
{
    FALSE = 0U,
    TRUE = 1U
} Ret_Val_e;

/*==================================================================================================
* Function: Ret_Val_e checkStringReverse(const char *originalStr, const char *reverseStr)
==================================================================================================*/
/**
 * @brief          Checks if two strings are reverse of each other.
 * @details        This function compares the given `originalStr` and `reverseStr` to 
 *                 determine if `reverseStr` is the reverse of `originalStr`. It first 
 *                 checks if the lengths of both strings are the same. If not, it returns 
 *                 FALSE. If the lengths are the same, it compares each character from 
 *                 `originalStr` to the corresponding character in `reverseStr` starting 
 *                 from the last character of `reverseStr`.
 *
 * @param[in]      originalStr   Pointer to the original string to be checked.
 * @param[in]      reverseStr    Pointer to the string that is expected to be the reverse 
 *                               of the original string.
 *
 * @return         Returns TRUE if `reverseStr` is the reverse of `originalStr`, 
 *                 otherwise returns FALSE.
 *
 * @note           The comparison is case-sensitive, so 'a' and 'A' are considered 
 *                 different characters.
 */
Ret_Val_e checkStringReverse(const char *originalStr, const char *reverseStr);

/*==================================================================================================
* Function: Ret_Val_e checkPrimeNumber(uint8_t number)
==================================================================================================*/
/**
 * @brief          Checks whether a given number is prime.
 * @details        This function determines if the input `number` is prime. It first checks if 
 *                 the number is less than or equal to 1 (not prime), or if it's exactly 2 (prime). 
 *                 For even numbers greater than 2, it returns FALSE since they're not prime. 
 *                 For odd numbers, it checks for divisibility by odd divisors starting from 3 
 *                 up to the square root of the number. If a divisor is found, the number is not prime.
 *
 * @param[in]      number   The number to check for primality (0 - 255).
 *
 * @return         Returns TRUE if the `number` is prime, otherwise returns FALSE.
 *
 * @note           0 and 1 are not prime numbers, and 2 is the only even prime number.
 */
Ret_Val_e checkPrimeNumber(uint8_t number);

/*==================================================================================================
* Function: void generateUniqueRandomNumbers(uint8_t numbersArray[], size_t size)
==================================================================================================*/
/**
 * @brief          Generate an array of unique random numbers between 1 and 100.
 * @details        This function generates `size` unique random numbers in the range 
 *                 [1, 100] and stores them in the given `numbersArray`. It uses 
 *                 the current time as the seed for the random number generator to 
 *                 ensure randomization. The function ensures that no duplicates 
 *                 are added to the array by maintaining a boolean array to track 
 *                 whether a number has already been used.
 *
 * @param[in,out]  numbersArray  Pointer to the array where the unique random numbers 
 *                               will be stored. This array must be pre-allocated 
 *                               and have a size of at least `size`.
 * @param[in]      size          The number of unique random numbers to generate. 
 *                               This should not exceed 100, as the range of numbers 
 *                               is limited to 1-100.
 *
 * @return         None.
 *
 * @note           If `size` exceeds 100, the function will enter an infinite loop 
 *                 because there aren't enough unique numbers in the range [1, 100] 
 *                 to satisfy the request.
 */
void generateUniqueRandomNumbers(uint8_t numbersArray[], size_t size);
#endif /* _AUTOTESTLIB_H_*/
/******************************************************************************
 * EOF
 ******************************************************************************/