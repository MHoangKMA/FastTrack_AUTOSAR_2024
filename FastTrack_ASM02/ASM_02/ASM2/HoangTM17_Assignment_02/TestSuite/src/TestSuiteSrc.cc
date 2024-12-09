 /*
 * filename: TestSuiteSrc.cc
 *
 *  Created on: Sep 05, 2024
 *      Author: Hoang Tran-Minh 
 *      Mail: HoangTM17@fpt.com
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "../../Testsuite/include/TestSuiteLib.h"

/*******************************************************************************
 * Testcase 01: TEST(TestSuiteAssignment_02, TestCase_01_ReverseStringTest)
 ******************************************************************************/
/**
* @brief          Test case for checking if a string is the reverse of another string.
* @details        This test case verifies if the `checkStringReverse` function correctly identifies 
*                 whether the second string is the reverse of the first string. It uses a sample 
*                 original string and its expected reversed string to validate the function's correctness.
* @test_procedure Steps:
*                   - Define the original string `strOriginal`.
*                   - Define the expected reversed string `strReverse`.
*                   - Call `checkStringReverse` with these strings.
*                   - Verify that the return value is `TRUE` indicating a match.
* @pass_criteria  The test passes if `checkStringReverse` returns `TRUE` for the given inputs.
*
* @note           Ensure that the `checkStringReverse` function is correctly implemented and linked.
*/
TEST(TestSuiteAssignment_02, TestCase_01_ReverseStringTest)
{
    /* Original string to be reversed */
    const char *strOriginal = "automation testing";
    
    /* Expected reversed string */
    const char *strReverse = "gnitset nnntamotua";
    
    /* Verify if the reversed string matches the expected result using checkStringReverse function */
    /* @step KEY VERIFICATION POINT: EXPECT_TRUE() expects return is TRUE */
    EXPECT_TRUE(checkStringReverse(strOriginal, strReverse));
}

/*******************************************************************************
 * Testcase 02: TEST(TestSuiteAssignment_02, TestCase_02_PrimeNumberTest)
 ******************************************************************************/ 
/**
* @brief          Test case for verifying if randomly generated numbers are prime.
* @details        This test case validates that each number in an array of 10 unique random numbers 
*                 is identified as a prime number by the `checkPrimeNumber` function. It ensures 
*                 that the function correctly determines the primality of each number generated.
* @test_procedure Steps:
*                   - Initialize an array `numbersArrayTest` to store 10 unique random numbers.
*                   - Generate 10 unique random numbers and populate the array using `generateUniqueRandomNumbers`.
*                   - Iterate through each number in the array.
*                   - Check if the current number is a prime number using `checkPrimeNumber`.
*                   - Verify that the `checkPrimeNumber` function returns `TRUE` for each number.
* @pass_criteria  The test passes if `checkPrimeNumber` returns `TRUE` for all numbers in the array.
*
* @note           Ensure that the `checkPrimeNumber` function is correctly implemented to check primality,
*                 and that `generateUniqueRandomNumbers` generates unique numbers as expected.
*/
TEST(TestSuiteAssignment_02, TestCase_02_PrimeNumberTest)
{
    /* Array to store 10 unique random numbers */
    uint8_t numbersArrayTest[10] = {0U}; 

    /* Generate 10 unique random numbers */
    generateUniqueRandomNumbers(numbersArrayTest, sizeof(numbersArrayTest) / sizeof(numbersArrayTest[0]));

    /* Check if each number in numbersArrayTest is a prime number */
    for (size_t i = 0; i < sizeof(numbersArrayTest) / sizeof(numbersArrayTest[0]); ++i)
    {
        /* Check if the current number is a prime number */
        bool isPrime = checkPrimeNumber(numbersArrayTest[i]);

        /* Expect that the number is a prime number */
        /* @step KEY VERIFICATION POINT: Expect that the number is a prime number */
        EXPECT_TRUE(isPrime) << "Number " << (int)numbersArrayTest[i] << " is not prime";
    }
}
 /******************************************************************************
 * EOF
 ******************************************************************************/