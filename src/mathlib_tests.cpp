/**
 * @file mathlib_tests.cpp
 * @author Stanislav Letaši
 * @brief Tests for math library
 * @date 24.3.2023
 */

#include "googletest-main/googletest/include/gtest/gtest.h"
#include <math.h>

extern "C"
{
#include "math_library.h"
}

using namespace ::testing;

class BasicTests : public Test
{
};

TEST_F(BasicTests, add)
{

    EXPECT_EQ(add(13, 0), 13);
    EXPECT_EQ(add(45, 28), 73);
    EXPECT_EQ(add(0, 0), 0);
    EXPECT_EQ(add(-77, -69), -146);
    EXPECT_EQ(add(-77, 69), -8);
    ASSERT_NEAR(9896.70434, add(42.25847L, 9854.44587L), 1e-8);
    ASSERT_NEAR(-709.89744, add(-13.89745L, -695.99999L), 1e-8);
    ASSERT_NEAR(-305.43198, add(28.4569L, -333.88888L), 1e-8);
    ASSERT_NEAR(23.3223344412L, add(11.12345678945876126L, 12.1988776517894278L), 1e-8);
}

TEST_F(BasicTests, sub)
{

    EXPECT_EQ(sub(13, 0), 13);
    EXPECT_EQ(sub(45, 28), 17);
    EXPECT_EQ(sub(0, 0), 0);
    EXPECT_EQ(sub(-77, -69), -8);
    EXPECT_EQ(sub(-77, 69), -146);
    ASSERT_NEAR(-9812.1874, sub(42.25847L, 9854.44587L), 1e-8);
    ASSERT_NEAR(682.10254, sub(-13.89745L, -695.99999L), 1e-8);
    ASSERT_NEAR(362.34578, sub(28.4569L, -333.88888L), 1e-8);
    ASSERT_NEAR(-1.07542086233L, sub(11.12345678945876126L, 12.1988776517894278L), 1e-8);
}

TEST_F(BasicTests, mul)
{

    EXPECT_EQ(mul(13, 0), 0);
    EXPECT_EQ(mul(45, 28), 1260);
    EXPECT_EQ(mul(0, 0), 0);
    EXPECT_EQ(mul(-77, -69), 5313);
    EXPECT_EQ(mul(-77, 69), -5313);
    ASSERT_NEAR(39065.6033528L, mul(42.2584L, 924.44587L), 1e-8);
    ASSERT_NEAR(9672.62506103L, mul(-13.89745L, -695.99999L), 1e-8);
    ASSERT_NEAR(-9501.44246927L, mul(28.4569L, -333.88888L), 1e-8);
    ASSERT_NEAR(135.69368844L, mul(11.12345678945876126L, 12.1988776517894278L), 1e-8);
}

TEST_F(BasicTests, div)
{

    EXPECT_EQ(divide(0.0, 96.0), 0);
    ASSERT_NEAR(1.60714285714L, divide(45, 28), 1e-8);
    ASSERT_NEAR(1.11594202899L, divide(-77, -69), 1e-8);
    ASSERT_NEAR(-1.11594202899L, divide(-77, 69), 1e-8);
    ASSERT_NEAR(0.00428826446L, divide(42.25847L, 9854.44587L), 1e-8);
    ASSERT_NEAR(0.01996760086L, divide(-13.89745L, -695.99999L), 1e-8);
    ASSERT_NEAR(-0.08522865451L, divide(28.4569L, -333.88888L), 1e-8);
    ASSERT_NEAR(0.91184263888L, divide(11.12345678945876126L, 12.1988776517894278L), 1e-8);
}

TEST_F(BasicTests, factorial)
{

    EXPECT_EQ(factorial(1), 1);
    EXPECT_EQ(factorial(0), 1);
    EXPECT_EQ(factorial(5), 120);
    EXPECT_EQ(factorial(7), 5040);
    EXPECT_EQ(factorial(15), 1307674368000);
    EXPECT_EQ(factorial(20), 2432902008176640000ul);
}

TEST_F(BasicTests, power)
{

    EXPECT_EQ(power(45, 2), 2025);
    EXPECT_EQ(power(-77, 5), -2706784157);
    EXPECT_EQ(power(-77, 4), 35153041);
    ASSERT_NEAR(0.00025005294L, power(0.12575L, 4L), 1e-8);
    ASSERT_NEAR(-23044.2598206L, power(-28.4569L, 3L), 1e-8);
    EXPECT_EQ(power(1, 200), 1);
    EXPECT_EQ(power(48, 0), 1);
}

TEST_F(BasicTests, root)
{

    EXPECT_EQ(root(1, 10), 1);
    EXPECT_EQ(root(0, 5), 0);
    ASSERT_NEAR(6.7082039325L, root(45, 2), 1e-8);
    ASSERT_NEAR(-1.44224957031L, root(-3, 3), 1e-8);
    ASSERT_NEAR(-2.38395550345L, root(-77, 5), 1e-8);
    ASSERT_NEAR(0.59549346304L, root(0.12575, 4), 1e-8);
    ASSERT_NEAR(-1.61339640149L, root(-28.4569, 7), 1e-8);
}

TEST_F(BasicTests, comb)
{

    EXPECT_EQ(comb(45, 2), 990);
    EXPECT_EQ(comb(77, 5), 19757815);
    EXPECT_EQ(comb(6, 6), 1);
    EXPECT_EQ(comb(25, 1), 25);
    EXPECT_EQ(comb(15, 0), 1);
    EXPECT_EQ(comb(0, 0), 1);
}