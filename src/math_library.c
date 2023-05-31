/**
 * @file math_library.c
 * @author František Holáň
 * @brief Math library implementation
 * @date 28.3.2023
 */

#include "math_library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long double add(long double x, long double y)
{
    long double result = x + y;

    return result;
}

long double sub(long double x, long double y)
{
    long double result = x - y;

    return result;
}

long double mul(long double x, long double y)
{
    long double result = x * y;

    return result;
}

long double divide(long double x, long double y)
{
    long double result = x / y;

    return result;
}

unsigned long long factorial(unsigned long x)
{

    unsigned long result = 1;

    for (unsigned long i = 1; i <= x; i++)
    {
        result *= i;
    }

    return result;
}

long double power(long double x, unsigned long y)
{
    long double result = 1;
    for (unsigned long i = 0; i < y; i++)
    {
        result = result * x;
    }

    return result;
}

long double root(long double x, unsigned long y)
{
    /*
        Newton-rapshon's method
        x(n+1) = x(n) - f(x)/f'(x)
        Next value:
        k(n+1) = 1/y*[(y-1)*k(n) + x/k(n)^(y-1)]
    */
    long double num = x / y; // initial value k
    long double var = 1.0L;

    if (x == 0)
    {
        return 0.0L;
    }

    while (var > 1e-8)
    {
        long double new_num = (((y - 1.0L) * num) + x / power(num, y - 1)) / y;
        var = num - new_num;

        if (var < 0.0)
        {
            var = var * (-1.0L);
        }

        num = new_num;
    }

    return num;
}

unsigned long comb(unsigned long x, unsigned long y)
{
    unsigned int comb_num[y + 1];

    memset(comb_num, 0, sizeof(comb_num)); // initialization

    comb_num[0] = 1;
    for (unsigned long j = 1; j <= x; j++)
    {
        int min = (j < y) ? j : y;

        // Calculation of another line of Pascal's triangle
        for (int k = min; k > 0; k--)
        {
            comb_num[k] += comb_num[k - 1];
        }
    }
    return comb_num[y];
}