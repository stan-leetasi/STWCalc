/**
 * @file math_library.h
 * @author Stanislav Letaši
 * @brief Header file for functions of math library
 * @date 20.3.2023
 */

#ifndef MATH_LIBRARY_H
#define MATH_LIBRARY_H

/**
 * @brief Sums up two numbers
 * @param x
 * @param y
 * @return x+y
 */
long double add(long double x, long double y);

/**
 * @brief Subtraction
 * @param x
 * @param y
 * @return x-y
 */
long double sub(long double x, long double y);

/**
 * @brief Product
 * @param x
 * @param y
 * @return x*y
 */
long double mul(long double x, long double y);

/**
 * @brief Decimal division
 * @param x
 * @param y
 * @return x/y
 */
long double divide(long double x, long double y);

/**
 * @brief Factorial
 * @param x
 * @return x!
 */
unsigned long long factorial(unsigned long x);

/**
 * @brief y-th power of x
 * @param x decimal number
 * @param y natural number
 * @return x**y
 */
long double power(long double x, unsigned long y);

/**
 * @brief y-th root of x
 * @param x decimal number
 * @param y natural number
 * @return y√x
 */
long double root(long double x, unsigned long y);

/**
 * @brief Binomial coefficient
 * @param x
 * @param y
 * @return xCy
 */
unsigned long comb(unsigned long x, unsigned long y);

#endif