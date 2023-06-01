/**
 * @file engine.h
 * @author Michal Krulich
 * @brief Public methods for the calculator's engine.
 * @date 3.4.2023
 *
 * Mimics the behaviour of the most simple calculators.
 * No operator precedence, remembers only the last evaluated value,...
 * 
 * All public methods that work on engine structure look like this:
 * action_t caleng_*(engine_t *eng, ...)
 *
 * They always return value of type action_t which holds the return code and text to be displayed.
 * If the return code is other than OK, the text to be displated is undefined. In that case GUI
 * should show its own error message based on the return code.
 */

#define CANCEL_CHAR 'C'
#define BACKSPACE_CHAR 'B'
#define POWER_TO_CHAR '^'
#define ROOT_CHAR 'R'
#define FACTORIAL_CHAR '!'
#define COMBINATION_CHAR 'K'

#define BUFFER_SIZE 100 // LENGTH of a regular buffer for number input
#define DEFAULT_MANTISSA_LENGTH_LIMIT 9
#define DEFAULT_EXPONENT_LENGTH_LIMIT 2

/**
 * @brief Identifiers for binary operations
 */
enum binary_ops
{
    NONE,
    EVAL,
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    ROOT,
    COMBINATIONAL
};
/**
 * @brief Identifiers for unary operations
 */
enum unary_ops
{
    FACT
};
/**
 * @brief Possible outcomes of all public methods of the engine
 */
enum result_rtn_types
{
    OK,
    OVERFLOW_ERR,
    SYNTAX_ERR,
    MATH_ERR
};

/** @struct cal_engine
 *  @brief Data structure of the engine.
 *  @param memory last saved processed value
 *  @param input_buffer string buffer for user number input
 *  @param sel_op last selected operation, possible values from binary_ops
 *  @param mantissa_length_limit maximum displayed length of mantissa
 *  @param exponent_length_limit maximum displayed length of exponent
 *  @param status return code of the last operation
 *  @param dp_sep decimal point character (based on user's current localisation settings)
 */
struct cal_engine
{
    long double memory;
    char input_buffer[BUFFER_SIZE + 1];
    int sel_op;
    int mantissa_length_limit;
    int exponent_length_limit;
    int status;
    char dp_sep;
};

/**
 * @brief Pair of values returned by every public method of the engine.
 * @param rtn_code Result of the action/method.
 * @param to_display String to be displayed in the GUI.
 */
struct action_result
{
    int rtn_code;
    char to_display[BUFFER_SIZE + 1];
};

typedef struct cal_engine engine_t;
typedef struct action_result result_t;
/**
 * @brief Initializes a new engine.
 *
 * @return Pointer to the engine.
 */
engine_t *caleng_init();

/**
 * @brief Destroys and frees the engine.
 *
 * @param eng Pointer to the engine
 */
void caleng_free(engine_t *eng);

/**
 * @brief Resets the calculator to the initial state.
 * @param eng Pointer to the engine.
 * @return struct action_result
 * @details Clears buffer, sets memory to 0 and sel_op to NONE.
 */
result_t caleng_cancel(engine_t *eng);

/**
 * @brief Clears the last character in the buffer.
 * @param eng Pointer to the engine.
 * @return struct action_result
 * @details The function has no effect if the buffer is empty.
 */
result_t caleng_backspace(engine_t *eng);

/**
 * @brief Inserts a digit into the buffer if the limit for mantissa or exponent was not reached.
 * @param eng Pointer to the engine.
 * @param digit Digit character '0' - '9'.
 * @return struct action_result
 * @details
 * If the first digit in the input buffer is 0 then the next digit replaces the first zero.
 * Last processed value is thrown away when user inputs something into the input buffer.
 * @todo Proper result printing
 */
result_t caleng_send_digit(engine_t *eng, char digit);

/**
 * @brief Inserts 'e' to the buffer and starts a mode for exponent input
 * @details
 * Inserts character of exponent 'e' in the input buffer.
 * The function has no effect if 'e' is already in the buffer.
 * If no digit is in the buffer, digit 1 is inserted before 'e'.
 * If no digit follows comma, comma is deleted.
 * Last processed value is thrown away when user inputs something into the input buffer.
 * @param eng Pointer to the engine.
 * @return struct action_result
 */
result_t caleng_insert_exp(engine_t *eng);

/**
 * @brief Inserts decimal point in the input buffer.
 * @details
 * Inserts character of decimal point ',' or '.' (based on your localisation) in the input buffer.
 * The function has no effect if decimal point or 'e' is already in the buffer.
 * If no digit is in the buffer, digit 0 is inserted before the deciaml point.
 * Last processed value is thrown away when user inputs something into the input buffer.
 * @param eng Pointer to the engine.
 * @return struct action_result
 */
result_t caleng_insert_decimal_point(engine_t *eng);

/**
 * @brief Negates mantissa or exponent if 'e' was already inserted.
 * @param eng Pointer to the engine.
 * @return struct action_result
 * @details Inserts or deletes the minus character '-'.
 */
result_t caleng_negate(engine_t *eng);

/**
 * @brief Evaluates the current given expression (based on eng->sel_op).
 * @details
 * If operation was not selected, the buffer is processed and stored in memory.
 * If operation was evaluate '=', this function has no effect.
 * Otherwise the expression is evaluated, result is stored in memory and sel_op set to EVAL.
 * If the second operand was not given, the first one will be used also as a second operand.
 * @param eng Pointer to the engine.
 * @return struct action_result
 */
result_t caleng_evaluate(engine_t *eng);

/**
 * @brief Performs an unary operation. Stores the result in engine's memory.
 * @details
 * If the last operation was evaluate '=', the value in memory will be used.
 * If an binary operator is already selected, the previous expresion will be evaluated first.
 * Otherwise the value in the input buffer will be used.
 * If the operation is succesful, sel_op is set to EVAL.
 * @param eng Pointer to the engine.
 * @param op Identifier of the operation (from enum unary_ops).
 * @return struct action_result
 */
result_t caleng_eval_un_op(engine_t *eng, int op);

/**
 * @brief Sets the binary operator in the calculator (eng->sel_op).
 * @details
 * If no operator was selected, the buffer will be processed and stored in memory.
 * If the last operation was evaluate '=', only the new operator is set.
 * Otherwise evaluates the previos expression if the buffer is not empty. If the buffer is empty, only changes the selected operator in th engine.
 * @param eng Pointer to the engine.
 * @param op Identifier of the operation (from enum binary_ops).
 * @return struct action_result
 */
result_t caleng_select_bi_op(engine_t *eng, int op);

/**
 * @brief Writes the value in engine's memory as a string to str_mem.
 * @param eng Pointer to the engine.
 * @param str_mem Position where the memory value should be written.
 */
void caleng_get_memory_string(engine_t *eng, char *str_mem);
