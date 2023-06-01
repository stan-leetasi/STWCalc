/**
 * @file engine.c
 * @author Michal Krulich
 * @brief Inner engine of the calculator
 * @date 20.4.2023
 */

#include "engine.h"
#include "math_library.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <locale.h>

/**
 * @brief Inserts a character on a given index.
 * @param str String to be changed.
 * @param c Character to be inserted.
 * @param index Position where the character should be inserted.
 */
void append_char_in_string(char *str, char c, int index)
{
    char tmp = str[index];
    str[index] = c;
    int i = index + 1;
    for (; str[i] != '\0'; i++)
    {
        c = str[i];
        str[i] = tmp;
        tmp = c;
    }
    str[i] = tmp;
    str[i + 1] = '\0';
}

/**
 * @brief Removes a character from string.
 * @param str String to be changed.
 * @param index Position of the character to be removed.
 */
void remove_char_in_string(char *str, int index)
{
    for (int i = index; str[i] != '\0'; i++)
    {
        str[i] = str[i + 1];
    }
    return;
}

/**
 * @brief Formats the string (usually input buffer) to an appropriate format for user
 * @param str String to be modified.
 * @details
 * If str is empty, '0' is added.
 * If str ends with "-" or "e", '0' is added.
 */
void caleng_format_display_input(char *str)
{
    if(strlen(str) == 0)
    {
        strcpy(str, "0");
        return;
    }
    char *pos;
    if((pos = strchr(str, 'e'))!=NULL)
    {
        if(pos[1] == '\0')
        {
            pos[1] = '0';
            pos[2] = '\0';
            return;
        }
    }
    pos = (pos == NULL) ? str : pos;
    if((pos = strchr(pos, '-')) != NULL)
    {
        if(pos[1] == '\0')
        {
            pos[1] = '0';
            pos[2] = '\0';
            return;
        }
    }
}

/**
 * @brief Processes the input buffer and returns a long double. Buffer is set to an empty string.
 * @details
 * This function uses sscanf to read a long double from the buffer, but before that the buffer must be checked
 * and modified if necessary, so the string is readable for sscanf. These procedures are made:
 * caleng_format_display_input is called on the input buffer,
 * if buffer ends with decimal point, the decimal point is removed.
 * @param eng Pointer to the engine.
 * @return Number read from the engine's input buffer
 */
long double caleng_process_input_buffer(engine_t *eng)
{
    assert(eng != NULL);

    caleng_format_display_input(eng->input_buffer);
    char *point = strchr(eng->input_buffer, eng->dp_sep);
    if(point != NULL)
    {
        if(point[1] == '\0')
        {
            point[0] = '\0';
        }
    }
    
    long double num;
    assert(1 == sscanf(eng->input_buffer, "%Lf", &num));
    eng->input_buffer[0] = '\0';
    return num;
}

/**
 * @brief Evaluates the selected binary operation (based on eng->sel_op), where the first operand is engine's memory.
 * Result is saved into engine's memory.
 * @param eng Pointer to the engine. Source of selected operation and first operand.
 * @param num Second operand.
 * @return Return code informing about success or error (from enum result_rtn_types).
 */
int caleng_eval_bi_op(engine_t *eng, long double num)
{
    long num_long, num_long2;
    switch (eng->sel_op)
    {
    case ADD:
        eng->memory = add(eng->memory, num);
        break;
    case SUB:
        eng->memory = sub(eng->memory, num);
        break;
    case MUL:
        eng->memory = mul(eng->memory, num);
        break;
    case DIV:
        if (num == 0.0)
        {
            return MATH_ERR;
        }
        eng->memory = divide(eng->memory, num);
        break;
    case POW:
        num_long = num;
        if (num_long < 0)
        {
            return MATH_ERR;
        }
        eng->memory = power(eng->memory, num);
        break;
    case ROOT:
        num_long = num;
        if (num_long <= 0)
        {
            return MATH_ERR;
        }
        if (eng->memory < 0.0 && num_long % 2 == 0)
        {
            return MATH_ERR;
        }
        eng->memory = root(eng->memory, num);
        break;

    case COMBINATIONAL:
        num_long = eng->memory;
        num_long2 = num;
        if (num_long < 0 || num_long2 < 0)
        {
            return MATH_ERR;
        }
        eng->memory = comb(num_long, num_long2);
        break;

    default:
        fprintf(stderr, "WARNING: caleng_eval_bi_op - invalid identifier\n");
        break;
    }
    if (eng->memory > 9.999999999e99 || eng->memory < -9.999999999e99)
    {
        return OVERFLOW_ERR;
    }
    return OK;
}

engine_t *caleng_init()
{
    engine_t *eng = malloc(sizeof(engine_t));
    if (eng != NULL)
    {
        eng->input_buffer[0] = '\0';
        eng->memory = 0.0;
        eng->sel_op = NONE;
        eng->mantissa_length_limit = DEFAULT_MANTISSA_LENGTH_LIMIT;
        eng->exponent_length_limit = DEFAULT_EXPONENT_LENGTH_LIMIT;
        eng->status = OK;
        eng->dp_sep = localeconv()->decimal_point[0];
    }
    return eng;
}

void caleng_free(engine_t *eng)
{
    free(eng);
}

result_t caleng_cancel(engine_t *eng)
{
    assert(eng != NULL);
    result_t r = {OK, "0"};
    eng->input_buffer[0] = '\0';
    eng->memory = 0.0;
    eng->sel_op = NONE;
    eng->status = OK;
    return r;
}

result_t caleng_backspace(engine_t *eng)
{
    assert(eng != NULL);
    result_t r = {OK, ""};
    if (eng->status != OK)
    {
        r.rtn_code = eng->status;
        return r;
    }

    if (eng->input_buffer[0] == '\0')
    {
        strcpy(r.to_display, "0");
        return r;
    }
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (eng->input_buffer[i] == '\0')
        {
            eng->input_buffer[i - 1] = '\0';
            break;
        }
    }
    if (eng->input_buffer[0] == '\0')
    {
        strcpy(r.to_display, "0");
        return r;
    }
    strcpy(r.to_display, eng->input_buffer);
    caleng_format_display_input(r.to_display);
    return r;
}

result_t caleng_send_digit(engine_t *eng, char digit)
{
    assert(eng != NULL);
    assert(digit >= '0' && digit <= '9');

    result_t r = {OK, ""};
    if (eng->status != OK)
    {
        r.rtn_code = eng->status;
        return r;
    }

    bool minus_inserted = false;
    bool exponent_mode = false;
    bool dpoint_inserted = false;
    int digits_detected = 0;
    int exponent_index = 0;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (eng->input_buffer[i] == '-')
        {
            minus_inserted = true;
        }
        else if (eng->input_buffer[i] == eng->dp_sep)
        {
            dpoint_inserted = true;
        }
        else if (eng->input_buffer[i] >= '0' && eng->input_buffer[i] <= '9')
        {
            digits_detected++; // counts the number of inserted digits
        }
        else if (eng->input_buffer[i] == 'e') // user already inputs exponent
        {
            exponent_mode = true;
            exponent_index = i;
            minus_inserted = false;
            dpoint_inserted = false;
            digits_detected = 0;
        }
        else if (eng->input_buffer[i] == '\0') // found the end of the input buffer
        {
            // Cases: "0", "-0" "...e0" "...e-0"
            if (eng->input_buffer[(minus_inserted ? 1 : 0) + exponent_index] == '0' && !dpoint_inserted)
            {
                eng->input_buffer[(minus_inserted ? 1 : 0) + exponent_index] = digit;
            }
            else if (exponent_mode)
            {
                if (digits_detected < eng->exponent_length_limit)
                {
                    eng->input_buffer[i] = digit;
                    eng->input_buffer[i + 1] = '\0';
                }
            }
            else
            {
                if (digits_detected < eng->mantissa_length_limit)
                {
                    eng->input_buffer[i] = digit;
                    eng->input_buffer[i + 1] = '\0';
                }
            }
            break;
        }
    }
    if (eng->sel_op == EVAL) // last processed value is thrown away when user inputs something into the input buffer
    {
        eng->sel_op = NONE;
    }
    strcpy(r.to_display, eng->input_buffer);
    caleng_format_display_input(r.to_display);
    return r;
}

result_t caleng_insert_exp(engine_t *eng)
{
    assert(eng != NULL);

    result_t r = {OK, ""};
    if (eng->status != OK)
    {
        r.rtn_code = eng->status;
        return r;
    }

    int non_zero_digits_detected = 0;
    // Check whether decimal point is followed by no digit.
    for(int i = 0; i < BUFFER_SIZE || eng->input_buffer[i] != '\0'; i++)
    {
        if (eng->input_buffer[i] == eng->dp_sep)
        {
            if(eng->input_buffer[i+1] == '\0')
            {
                caleng_backspace(eng);
            }
            break;
        }
    }
    // exponent insert
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (eng->input_buffer[i] == 'e')
        {
            break;
        }
        else if (eng->input_buffer[i] > '0' && eng->input_buffer[i] <= '9')
        {
            non_zero_digits_detected++;
        }
        else if (eng->input_buffer[i] == '\0')
        {
            // no non-zero digit was inserted --- cases: "", "0", "-0" "...e0" "...e-0"
            if (non_zero_digits_detected == 0)
            {
                caleng_send_digit(eng, '1');
                i = strlen(eng->input_buffer);
            }
            eng->input_buffer[i] = 'e';
            eng->input_buffer[i + 1] = '\0';
            break;
        }
    }
    if (eng->sel_op == EVAL) // last processed value is thrown away when user inputs something into the input buffer
    {
        eng->sel_op = NONE;
    }
    strcpy(r.to_display, eng->input_buffer);
    caleng_format_display_input(r.to_display);
    return r;
}

result_t caleng_insert_decimal_point(engine_t *eng)
{
    assert(eng != NULL);
    result_t r = {OK, ""};
    if (eng->status != OK)
    {
        r.rtn_code = eng->status;
        return r;
    }

    int non_zero_digits_detected = 0;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (eng->input_buffer[i] == eng->dp_sep || eng->input_buffer[i] == 'e')
        {
            break;
        }
        else if (eng->input_buffer[i] > '0' && eng->input_buffer[i] <= '9')
        {
            non_zero_digits_detected++;
        }
        else if (eng->input_buffer[i] == '\0')
        {
            if (non_zero_digits_detected == 0)
            {
                caleng_send_digit(eng, '0');
            }
            i = strlen(eng->input_buffer);
            eng->input_buffer[i] = eng->dp_sep;
            eng->input_buffer[i + 1] = '\0';
            break;
        }
    }
    if (eng->sel_op == EVAL) // last processed value is thrown away when user inputs something into the input buffer
        eng->sel_op = NONE;
    strcpy(r.to_display, eng->input_buffer);
    caleng_format_display_input(r.to_display);
    return r;
}

result_t caleng_negate(engine_t *eng)
{
    assert(eng != NULL);
    result_t r = {OK, ""};
    if (eng->status != OK)
    {
        r.rtn_code = eng->status;
        return r;
    }

    // decide whether to insert/delete '-' in mantissa or exponent
    bool exponent_mode = false;
    int i = 0;
    for (; i < BUFFER_SIZE && eng->input_buffer[i] != '\0'; i++)
    {
        if (eng->input_buffer[i] == 'e')
        {
            exponent_mode = true;
            break;
        }
    }
    if (exponent_mode)
    {
        if (eng->input_buffer[i + 1] == '-')
        {
            remove_char_in_string(eng->input_buffer, i + 1);
        }
        else
        {
            append_char_in_string(eng->input_buffer, '-', i + 1);
        }
    }
    else
    {
        if (eng->input_buffer[0] == '-')
        {
            remove_char_in_string(eng->input_buffer, 0);
        }
        else
        {
            append_char_in_string(eng->input_buffer, '-', 0);
        }
    }
    if (eng->sel_op == EVAL) // last processed value is thrown away when user inputs something into the input buffer
    {
        eng->sel_op = NONE;
    }
    strcpy(r.to_display, eng->input_buffer);
    caleng_format_display_input(r.to_display);
    return r;
}

result_t caleng_evaluate(engine_t *eng)
{
    assert(eng != NULL);
    result_t r = {OK, ""};
    if (eng->status != OK)
    {
        r.rtn_code = eng->status;
        return r;
    }

    if (eng->sel_op == NONE || eng->sel_op == EVAL)
    {
        if (eng->sel_op == NONE)
        {
            eng->memory = caleng_process_input_buffer(eng);
        }
        caleng_get_memory_string(eng, r.to_display);
    }
    else if (eng->input_buffer[0] == '\0')
    {
        r.rtn_code = caleng_eval_bi_op(eng, eng->memory);
        if (r.rtn_code != OK)
        {
            eng->status = r.rtn_code;
            return r;
        }
        caleng_get_memory_string(eng, r.to_display);
    }
    else
    {
        r.rtn_code = caleng_eval_bi_op(eng, caleng_process_input_buffer(eng));
        if (r.rtn_code != OK)
        {
            eng->status = r.rtn_code;
            return r;
        }
        caleng_get_memory_string(eng, r.to_display);
    }
    eng->sel_op = EVAL; 
    return r;
}

result_t caleng_eval_un_op(engine_t *eng, int op)
{
    assert(eng != NULL);
    result_t r = {OK, ""};
    if (eng->status != OK)
    {
        r.rtn_code = eng->status;
        return r;
    }

    if (eng->sel_op == NONE)
    {
        // use value from the input buffer
        eng->memory = caleng_process_input_buffer(eng);
    }
    else if (eng->sel_op == EVAL && eng->input_buffer[0] == '\0')
    {
        // use memory
    }
    else
    {
        // evaluate previous expression
        r = caleng_evaluate(eng);
    }
    
    if (r.rtn_code == OK)
    {
        switch (op)
        {
        case FACT:
            long num = eng->memory;
            if (num < 0)
            {
                r.rtn_code = MATH_ERR;
                break;
            }
            if (num > 20)
            {
                r.rtn_code = OVERFLOW_ERR;
                break;
            }
            eng->memory = factorial(num);
            break;
        default:
            fprintf(stderr, "WARNING: caleng_eval_un_op - invalid identifier\n");
            break;
        }
    }

    if (r.rtn_code != OK)
    {
        eng->status = r.rtn_code;
    }
    else
    {
        caleng_get_memory_string(eng, r.to_display);
        eng->sel_op = EVAL;
    }

    return r;
}

result_t caleng_select_bi_op(engine_t *eng, int op)
{
    assert(eng != NULL);
    result_t r = {OK, ""};
    if (eng->status != OK)
    {
        r.rtn_code = eng->status;
        return r;
    }
    if (eng->sel_op == NONE)
    {
        eng->sel_op = op;
        eng->memory = caleng_process_input_buffer(eng);
    }
    else if (eng->sel_op == EVAL)
    {
        eng->sel_op = op;
    }
    else
    {
        if (eng->input_buffer[0] == '\0')
        {
            eng->sel_op = op;
        }
        else
        {
            r.rtn_code = caleng_eval_bi_op(eng, caleng_process_input_buffer(eng));
            eng->sel_op = op;
        }
    }
    caleng_get_memory_string(eng, r.to_display);
    return r;
}

void caleng_get_memory_string(engine_t *eng, char *str_mem)
{
    double num = eng->memory;
    sprintf(str_mem, "%g", num);
}