/**
 * @file engine_tests.cpp
 * @author Michal Krulich
 * @brief Tests for calculator's engine
 * @date 31.5.2023
 */

#include "googletest-main/googletest/include/gtest/gtest.h"

extern "C"
{
#include "engine.h"
#include <locale.h>
}

using namespace ::testing;

class EngineTest : public Test
{
    public:
        engine_t *eng;
    void SetUp()
    {
        setlocale(LC_ALL, "en_US.UTF-8");
        eng = caleng_init();
    }
    void TearDown()
    {
        caleng_free(eng);
    }
};

TEST_F(EngineTest, caleng_init)
{
    EXPECT_NE(eng, nullptr);
    EXPECT_EQ(eng->memory, 0.0);
    EXPECT_STREQ(eng->input_buffer, "");
}

TEST_F(EngineTest, caleng_cancel)
{
    EXPECT_STREQ("5", caleng_insert_digit(eng, '5').to_display);
    EXPECT_STREQ("54", caleng_insert_digit(eng, '4').to_display);
    EXPECT_STREQ("0", caleng_cancel(eng).to_display);
    EXPECT_STREQ("3", caleng_insert_digit(eng, '3').to_display);
}

TEST_F(EngineTest, caleng_backspace)
{
    EXPECT_STREQ("0", caleng_backspace(eng).to_display);
    EXPECT_STREQ("1", caleng_insert_digit(eng, '1').to_display);
    EXPECT_STREQ("12", caleng_insert_digit(eng, '2').to_display);
    EXPECT_STREQ("120", caleng_insert_digit(eng, '0').to_display);
    EXPECT_STREQ("12", caleng_backspace(eng).to_display);
    EXPECT_STREQ("1", caleng_backspace(eng).to_display);
    EXPECT_STREQ("0", caleng_backspace(eng).to_display);
    EXPECT_STREQ("", eng->input_buffer);
}

TEST_F(EngineTest, caleng_insert_digit)
{
    EXPECT_STREQ("1", caleng_insert_digit(eng, '1').to_display);
    EXPECT_STREQ("12", caleng_insert_digit(eng, '2').to_display);
    EXPECT_STREQ("120", caleng_insert_digit(eng, '0').to_display);
    EXPECT_STREQ("1200", caleng_insert_digit(eng, '0').to_display);
    EXPECT_STREQ("12009", caleng_insert_digit(eng, '9').to_display);

    EXPECT_STREQ("0", caleng_cancel(eng).to_display);
    EXPECT_STREQ("", eng->input_buffer);
    EXPECT_STREQ("0", caleng_insert_digit(eng, '0').to_display);
    EXPECT_STREQ("0", caleng_insert_digit(eng, '0').to_display);
    EXPECT_STREQ("0", caleng_insert_digit(eng, '0').to_display);
    EXPECT_STREQ("1", caleng_insert_digit(eng, '1').to_display);
    EXPECT_STREQ("10", caleng_insert_digit(eng, '0').to_display);
    EXPECT_STREQ("100", caleng_insert_digit(eng, '0').to_display);
    EXPECT_STREQ("1000", caleng_insert_digit(eng, '0').to_display);
    EXPECT_STREQ("10000", caleng_insert_digit(eng, '0').to_display);
    EXPECT_STREQ("100000", caleng_insert_digit(eng, '0').to_display);
    EXPECT_STREQ("1000000", caleng_insert_digit(eng, '0').to_display);
    EXPECT_STREQ("10000009", caleng_insert_digit(eng, '9').to_display);
    EXPECT_STREQ("100000099", caleng_insert_digit(eng, '9').to_display);
    EXPECT_STREQ("100000099", caleng_insert_digit(eng, '8').to_display);
    EXPECT_STREQ("100000099", caleng_insert_digit(eng, '0').to_display);

    EXPECT_STREQ("0", caleng_cancel(eng).to_display);
    EXPECT_STREQ("", eng->input_buffer);
}

TEST_F(EngineTest, caleng_insert_exp)
{
    EXPECT_STREQ("1e0", caleng_insert_exp(eng).to_display);
    EXPECT_STREQ("1e", eng->input_buffer);
    EXPECT_STREQ("1e0", caleng_insert_exp(eng).to_display);
    EXPECT_STREQ("1", caleng_backspace(eng).to_display);
    EXPECT_STREQ("0", caleng_backspace(eng).to_display);
    EXPECT_STREQ("0.", caleng_insert_decimal_point(eng).to_display);
    EXPECT_STREQ("1e0", caleng_insert_exp(eng).to_display);
    EXPECT_STREQ("1e2", caleng_insert_digit(eng, '2').to_display);
    EXPECT_STREQ("1e28", caleng_insert_digit(eng, '8').to_display);
    EXPECT_STREQ("1e28", caleng_insert_digit(eng, '8').to_display);
    EXPECT_STREQ("1e28", caleng_insert_exp(eng).to_display);
    caleng_cancel(eng);
    caleng_insert_digit(eng, '4');
    caleng_insert_digit(eng, '0');
    EXPECT_STREQ("40e0", caleng_insert_exp(eng).to_display);
    EXPECT_STREQ("40e", eng->input_buffer);
}

TEST_F(EngineTest, caleng_insert_decimal_point)
{
    EXPECT_STREQ("0.", caleng_insert_decimal_point(eng).to_display);
    EXPECT_STREQ("0.2", caleng_insert_digit(eng, '2').to_display);
    EXPECT_STREQ("0.2", caleng_insert_decimal_point(eng).to_display);
    EXPECT_STREQ("0.2e0", caleng_insert_exp(eng).to_display);
    EXPECT_STREQ("0.2e7", caleng_insert_digit(eng, '7').to_display);

    caleng_cancel(eng);
    EXPECT_STREQ("-0", caleng_negate(eng).to_display);
    EXPECT_STREQ("-0.", caleng_insert_decimal_point(eng).to_display);
}

TEST_F(EngineTest, caleng_negate)
{
    EXPECT_STREQ("-0", caleng_negate(eng).to_display);
    EXPECT_STREQ("-", eng->input_buffer);
    EXPECT_STREQ("0", caleng_backspace(eng).to_display);
    EXPECT_STREQ("", eng->input_buffer);
    EXPECT_STREQ("7", caleng_insert_digit(eng, '7').to_display);
    EXPECT_STREQ("-7", caleng_negate(eng).to_display);
    EXPECT_STREQ("-77", caleng_insert_digit(eng, '7').to_display);
    EXPECT_STREQ("77", caleng_negate(eng).to_display);
    EXPECT_STREQ("775", caleng_insert_digit(eng, '5').to_display);
    EXPECT_STREQ("775.", caleng_insert_decimal_point(eng).to_display);
    EXPECT_STREQ("-775.", caleng_negate(eng).to_display);
    EXPECT_STREQ("-775.1", caleng_insert_digit(eng, '1').to_display);
    EXPECT_STREQ("-775.1e0", caleng_insert_exp(eng).to_display);
    EXPECT_STREQ("-775.1e-0", caleng_negate(eng).to_display);
    EXPECT_STREQ("-775.1e0", caleng_negate(eng).to_display);
    EXPECT_STREQ("-775.1e4", caleng_insert_digit(eng, '4').to_display);
    EXPECT_STREQ("-775.1e40", caleng_insert_digit(eng, '0').to_display);
    EXPECT_STREQ("-775.1e-40", caleng_negate(eng).to_display);
    EXPECT_STREQ("-775.1e40", caleng_negate(eng).to_display);
    EXPECT_STREQ("-775.1e40", caleng_insert_digit(eng, '2').to_display);
}

TEST_F(EngineTest, caleng_evaluate)
{
    EXPECT_STREQ("0", caleng_evaluate(eng).to_display);
    caleng_cancel(eng);
    // 4 + 6 = 
    EXPECT_STREQ("4", caleng_insert_digit(eng, '4').to_display);
    EXPECT_STREQ("4", caleng_select_bi_op(eng, ADD).to_display);
    EXPECT_STREQ("6", caleng_insert_digit(eng, '6').to_display);
    EXPECT_STREQ("10", caleng_evaluate(eng).to_display);
    // * 2 =
    EXPECT_STREQ("10", caleng_select_bi_op(eng, MUL).to_display);
    EXPECT_STREQ("2", caleng_insert_digit(eng, '2').to_display);
    EXPECT_STREQ("20", caleng_evaluate(eng).to_display);

    // 4 / =
    EXPECT_STREQ("4", caleng_insert_digit(eng, '4').to_display);
    EXPECT_STREQ("4", caleng_select_bi_op(eng, DIV).to_display);
    EXPECT_STREQ("1", caleng_evaluate(eng).to_display);
}

TEST_F(EngineTest, caleng_eval_un_op)
{
    caleng_insert_digit(eng, '4');
    EXPECT_STREQ("24", caleng_eval_un_op(eng, FACT).to_display);
    caleng_cancel(eng);
    caleng_insert_digit(eng, '3');
    EXPECT_STREQ("6", caleng_eval_un_op(eng, FACT).to_display);
    EXPECT_STREQ("720", caleng_eval_un_op(eng, FACT).to_display);
}

TEST_F(EngineTest, caleng_select_bi_op)
{
    EXPECT_STREQ("0", caleng_select_bi_op(eng, ADD).to_display);
    EXPECT_STREQ("5", caleng_insert_digit(eng, '5').to_display);
    EXPECT_STREQ("5", caleng_select_bi_op(eng, MUL).to_display);
    EXPECT_STREQ("2", caleng_insert_digit(eng, '2').to_display);
    EXPECT_STREQ("-2", caleng_negate(eng).to_display);
    EXPECT_STREQ("-22", caleng_insert_digit(eng, '2').to_display);
    EXPECT_STREQ("-110", caleng_select_bi_op(eng, DIV).to_display);
    EXPECT_STREQ("2", caleng_insert_digit(eng, '2').to_display);
    EXPECT_STREQ("-55", caleng_select_bi_op(eng, DIV).to_display);
    EXPECT_STREQ("0", caleng_insert_digit(eng, '0').to_display);
    EXPECT_EQ(MATH_ERR, caleng_select_bi_op(eng, SUB).rtn_code);
}