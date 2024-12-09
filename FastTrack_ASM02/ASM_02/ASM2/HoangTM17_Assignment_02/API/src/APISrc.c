 /*
 * filename: APIsrc.c
 *
 *  Created on: Sep 05, 2024
 *      Author: Hoang Tran-Minh 
 *      Mail: HoangTM17@fpt.com
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "../../API/inlcude/APILib.h"

/*******************************************************************************
 * APIs
 ******************************************************************************/

/*==================================================================================================
* Function: Ret_Val_e checkStringReverse(const char *originalStr, const char *reverseStr)
==================================================================================================*/
/**
 * @brief          Checks if two strings are reverse of each other.               
 */
Ret_Val_e checkStringReverse(const char *originalStr, const char *reverseStr)
{
    /* Get the length of the original string and cast it to uint8_t */
    uint8_t originalStrLen = (uint8_t)strlen(originalStr);
    
    /* Get the length of the reverse string and cast it to uint8_t */
    uint8_t reverseStrLen  = (uint8_t)strlen(reverseStr);
    
    /* Variable to iterate through the strings */
    uint8_t indexCount = 0U;

    /* Init return value to TRUE, assuming strings are reverse by default */
    Ret_Val_e retValue = TRUE;  

    /* Check if the lengths of the two strings are not equal */
    if(originalStrLen != reverseStrLen)
    {
        /* If lengths are not equal, set return value to FALSE */
        retValue = FALSE;
    }
    else
    {
        /* Loop through each character in the original string */
        for(indexCount = 0U; indexCount < originalStrLen; ++indexCount)
        {
            /* Compare character from originalStr 
            with the corresponding character from reverseStr */
            if(originalStr[indexCount] != reverseStr[originalStrLen - 1U - indexCount])
            {
                /* If a mismatch is found, set return value to FALSE and exit the loop */
                retValue = FALSE;
                break; 
            }
        }
    }

    /* Return whether the strings are reverse of each other (TRUE/FALSE) */
    return retValue;
}

/*==================================================================================================
* Function: Ret_Val_e checkPrimeNumber(uint8_t number)
==================================================================================================*/
/**
 * @brief          Checks whether a given number is prime.
 */
Ret_Val_e checkPrimeNumber(uint8_t number)
{
    /* Initialize divisor to 3, to start checking from the smallest odd number */
    uint8_t divisor = 3U;
    
    /* Assume the number is prime by default */
    Ret_Val_e retValue = TRUE;  

    /* Check if the number is less than or equal to 1, which is not prime */
    if (number <= 1U)
    {
        /* If number is <= 1, it's not prime */
        retValue = FALSE;
    }
    /* Check if the number is exactly 2, which is a prime number */
    else if (number == 2U)
    {
        /* 2 is prime */
        retValue = TRUE;
    }
    /* Check if the number is even (except 2), which is not prime */
    else if ((number % 2U) == 0U)
    {
        /* Even numbers (except 2) are not prime */
        retValue = FALSE;
    }
    else
    {
        /* Check for divisibility by odd numbers starting from 3 
        up to the square root of the number */
        for (divisor = 3U; (divisor * divisor) <= number; divisor += 2U)
        {
            /* If the number is divisible by the current divisor, it's not prime */
            if ((number % divisor) == 0U)
            {
                retValue = FALSE;
                break;  /* Exit the loop if a divisor is found */
            }
        }
    }

    /* Return the result (TRUE if prime, FALSE otherwise) */
    return retValue;
}

/*==================================================================================================
* Function: void generateUniqueRandomNumbers(uint8_t numbersArray[], size_t size)
==================================================================================================*/
/**
 * @brief          Generate an array of unique random numbers between 1 and 100.
 */
void generateUniqueRandomNumbers(uint8_t numbersArray[], size_t size)
{
    /* Seed the random number generator with the current time */
    srand((unsigned)time(NULL));

    /* Array to keep track of used numbers from 1 to 100 */
    bool isNumberUsed[101] = {false}; 
    
    /* Index for array operations */
    size_t indexArray = 0U; 

    /* Fill numbersArray with unique random numbers between 1 and 100 */
    for (indexArray = 0U; indexArray < size; ++indexArray)
    {
        uint8_t num = 0U;
        do
        {
            /* Generate a random number between 1 and 100 */
            num = rand() % 100 + 1;

        } while (isNumberUsed[num]); 

        /* Assign the number to the array and mark it as used */
        numbersArray[indexArray] = num;
        isNumberUsed[num] = true; 
    }
}
/******************************************************************************
 * EOF
 ******************************************************************************/