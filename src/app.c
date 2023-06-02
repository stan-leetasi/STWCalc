/**
 * @file app.c
 * @author Michal Krulich
 * @brief GTK+ GUI interface for the calculator
 * @date 17.4.2023
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "engine.h"

/**
 * @brief inner engine
*/
engine_t *CALC_ENGINE;

/**
 * @brief main calcualtor's display
*/
GtkWidget *GLOBAL_DISPLAY;
/**
 * @brief label showing help
*/
GtkWidget *GLOBAL_HELP_LABEL;

/**
 * @brief Prints the given string on the main display.
 * @details Uses markup to format the output.
 * @param string String to be displayed
 */
void change_text_in_display(char *string);

/**
 * @brief Prints out the result of the operation on the main display based on the success of the operation.
 * @details If the operation's return is OK, numeric value / result is displayed.
 * Otherwise the error code is displayed.
 * @param result Result of the last operation.
 */
void change_text_based_on_result(result_t result);

/**
 * @brief App startup.
 * @warning Name of this function cannot be modified due to GTK's implementation.
 */
static void activate(GtkApplication *app, gpointer data);

/**
 * @brief Resets the calculator to default state.
 * Activated through button click.
 */
static void gui_cancel(GtkWidget *widget, gpointer data);

/**
 * @brief Vymaže posledný zadáný znak
 * Activated through button click.
 */
static void gui_backspace(GtkWidget *widget, gpointer data);

/**
 * @brief Inserts a digit.
 * @param digit inserted digit
 * Activated through button click.
 */
static void gui_send_digit(GtkWidget *widget, char digit);

/**
 * @brief Zero is inserted three times.
 * Activated through button click.
 */
static void gui_send_three_zeros(GtkWidget *widget, gpointer data);

/**
 * @brief Starts exponent insertion.
 * Activated through button click.
 */
static void gui_insert_exp(GtkWidget *widget, gpointer data);

/**
 * @brief Inserts decimal point.
 * Activated through button click.
 */
static void gui_insert_comma(GtkWidget *widget, gpointer data);

/**
 * @brief Negates mantissa or exponent.
 * Activated through button click.
 */
static void gui_negate(GtkWidget *widget, gpointer data);

/**
 * @brief Evaluates the current expression.
 * Activated through button click.
 */
static void gui_evaluate(GtkWidget *widget, gpointer data);

/**
 * @brief Selects and performs the current unary operation.
 * @param un_op identifier of the unary operation
 * Activated through button click.
 */
static void gui_eval_un_op(GtkWidget *widget, int un_op);

/**
 * @brief Selects binary operation.
 * @param bi_op identifier of the binary operation
 * Activated through button click.
 */
static void gui_select_bi_op(GtkWidget *widget, int bi_op);

/**
 * @brief Exponentiates a number.
 * @param value principal
 * Activated through button click.
 */
static void gui_power_to(GtkWidget *widget, int value);

/**
 * @brief Perform n-th root
 * @param value n
 * Activated through button click.
 */
static void gui_root_of(GtkWidget *widget, int value);

/**
 * @brief Handler for keyboard input.
 */
static gboolean key_press_cb(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer data);

/**
 * @brief Changes text in help label.
 * @param controller Controller calling this function.
 * @param data String to be displayed.
 */
static void change_help_description(GtkEventController *controller, gpointer data);

/**
 * @brief Text is diplayed in the help label when mouse cursor is over the given widget.
 * @details Creates a new GtkEventControllerMotion and connects it with the widget.
 * @param widget Target widget
 * @param text Text for help label
*/
#define connect_hover_action_to_help_change(widget, text)                                          \
    do                                                                                             \
    {                                                                                              \
        GtkEventController *motion_controller = gtk_event_controller_motion_new();           \
        g_signal_connect(motion_controller, "enter", G_CALLBACK(change_help_description), (text)); \
        gtk_widget_add_controller(GTK_WIDGET((widget)), motion_controller);                        \
    } while (0);

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, ""); // sets the user's localisation
    CALC_ENGINE = caleng_init();
    if (CALC_ENGINE == NULL)
    {
        fprintf(stderr, "caleng_init - memory allocation error\n");
        return EXIT_FAILURE;
    }
    GtkApplication *app = gtk_application_new("calculator.STWorks", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    // app start
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app); // free memory
    caleng_free(CALC_ENGINE);
    return status;
}

static void activate(GtkApplication *app, gpointer data)
{
    GtkWidget *window, *grid,
        *frame_display, *display,
        *btn_cancel, *btn_backspace,
        *btn_add, *btn_sub, *btn_mul, *btn_div, *btn_rtn,
        *btn_neg, *btn_exp,
        *btn_pow, *btn_pow2, *btn_pow3,
        *btn_root, *btn_root_square, *btn_root_cubic,
        *btn_fact, *btn_combination,
        *btn_digit1, *btn_digit2, *btn_digit3,
        *btn_digit4, *btn_digit5, *btn_digit6,
        *btn_digit7, *btn_digit8, *btn_digit9,
        *btn_digit0, *btn_thousand000, *btn_fl_point,
        *help_label;

    // create a window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "ST Works - Calculator");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE); // window size cannot be changed

    // container (grid) for widgets
    grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), grid);

    // display showing results
    frame_display = gtk_frame_new(NULL);
    display = gtk_label_new(NULL);
    GLOBAL_DISPLAY = display;
    gtk_frame_set_child(GTK_FRAME(frame_display), display);
    // align the label to the center left    
    gtk_widget_set_halign(display, GTK_ALIGN_END);      
    gtk_widget_set_valign(display, GTK_ALIGN_CENTER);   
    gtk_label_set_selectable(GTK_LABEL(display), TRUE); // text inside the label can be selected and copied
    gtk_widget_set_focusable(display, FALSE);           // text inside the label won't be focused at startup
    gtk_label_set_justify(GTK_LABEL(display), GTK_JUSTIFY_RIGHT);

    // buttons
    btn_cancel = gtk_button_new_with_label("C");
    btn_backspace = gtk_button_new_with_label("\u232B");
    btn_add = gtk_button_new_with_label("+"); // basic operations
    btn_sub = gtk_button_new_with_label("-");
    btn_mul = gtk_button_new_with_label("*");
    btn_div = gtk_button_new_with_label("/");
    btn_rtn = gtk_button_new_with_label("=");
    btn_pow = gtk_button_new_with_label("x\u207F"); // advanced operations
    btn_pow2 = gtk_button_new_with_label("x\u00B2");
    btn_pow3 = gtk_button_new_with_label("x\u00B3");
    btn_root = gtk_button_new_with_label("\u207F\u221Ax");
    btn_root_square = gtk_button_new_with_label("\u221Ax");
    btn_root_cubic = gtk_button_new_with_label("\u221Bx");
    btn_fact = gtk_button_new_with_label("x!");
    btn_combination = gtk_button_new_with_label("n\u2201k");
    btn_digit1 = gtk_button_new_with_label("1"); // number input
    btn_digit2 = gtk_button_new_with_label("2");
    btn_digit3 = gtk_button_new_with_label("3");
    btn_digit4 = gtk_button_new_with_label("4");
    btn_digit5 = gtk_button_new_with_label("5");
    btn_digit6 = gtk_button_new_with_label("6");
    btn_digit7 = gtk_button_new_with_label("7");
    btn_digit8 = gtk_button_new_with_label("8");
    btn_digit9 = gtk_button_new_with_label("9");
    btn_digit0 = gtk_button_new_with_label("0");
    btn_thousand000 = gtk_button_new_with_label("\u2080\u2080\u2080");
    btn_fl_point = gtk_button_new_with_label(".");
    btn_neg = gtk_button_new_with_label("\u00B1");
    btn_exp = gtk_button_new_with_label("exp");

    // label for help
    help_label = gtk_label_new("Hints are shown here");
    GLOBAL_HELP_LABEL = help_label;

    // Set glboal widget styles
    gtk_widget_set_name(btn_rtn, "btn_blue"); // different subgroups
    gtk_widget_set_name(btn_cancel, "btn_red");
    gtk_widget_set_name(btn_backspace, "btn_orange");

    GtkCssProvider *css_style_provider = gtk_css_provider_new();
    const gchar *css_parameters = "button { font-size: 22pt; min-width: 68px; min-height: 68px; padding: 1px; }\n"
                                  "#btn_orange { background-color: #ffad1a; }\n"
                                  "#btn_orange:hover { background-color: #Ffcf07; }\n"
                                  "#btn_orange:active { background-color: #cc8a15; }\n"
                                  "#btn_red { background-color: #C92525; }\n"
                                  "#btn_red:hover { background-color: #E20707; }\n"
                                  "#btn_red:active { background-color: #8d1a1a; }\n"
                                  "#btn_blue { background-color: #1a6cff; }\n"
                                  "#btn_blue:hover { background-color: #008efd; }\n"
                                  "#btn_blue:active { background-color: #104199; }\n"
                                  "label { min-height: 20px;  }\n"
                                  "headerbar entry,headerbar spinbutton,headerbar button,headerbar separator { margin-top: 0px; margin-bottom: 0px; min-height: 30px; min-width: 30px; }\n"
                                  "headerbar { min-height: 20px; padding-left: 2px; padding-right: 2px; margin: 0px; padding: 0px; } ";
    gtk_css_provider_load_from_data(css_style_provider, css_parameters, -1);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(css_style_provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Placing widgets into the grid
    gtk_grid_attach(GTK_GRID(grid), frame_display, 1, 1, 5, 1);

    gtk_grid_attach(GTK_GRID(grid), btn_cancel, 5, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_backspace, 4, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), btn_root_square, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_root_cubic, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_root, 3, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_pow2, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_pow3, 2, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_pow, 3, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_fact, 4, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_combination, 5, 4, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), btn_add, 4, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_sub, 5, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_mul, 4, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_div, 5, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_neg, 4, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_exp, 4, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_rtn, 5, 7, 1, 2);

    gtk_grid_attach(GTK_GRID(grid), btn_digit7, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_digit8, 2, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_digit9, 3, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_digit4, 1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_digit5, 2, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_digit6, 3, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_digit1, 1, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_digit2, 2, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_digit3, 3, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_digit0, 1, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_thousand000, 2, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_fl_point, 3, 8, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), help_label, 1, 9, 5, 1);

    // Sets a fixed size of the calculator's display
    gtk_widget_set_size_request(display, gtk_widget_get_size(display, GTK_ORIENTATION_HORIZONTAL), 80);

    // connection between the engine and buttons
    g_signal_connect(btn_cancel, "clicked", G_CALLBACK(gui_cancel), NULL);
    g_signal_connect(btn_backspace, "clicked", G_CALLBACK(gui_backspace), NULL);
    g_signal_connect(btn_digit0, "clicked", G_CALLBACK(gui_send_digit), '0');
    g_signal_connect(btn_digit1, "clicked", G_CALLBACK(gui_send_digit), '1');
    g_signal_connect(btn_digit2, "clicked", G_CALLBACK(gui_send_digit), '2');
    g_signal_connect(btn_digit3, "clicked", G_CALLBACK(gui_send_digit), '3');
    g_signal_connect(btn_digit4, "clicked", G_CALLBACK(gui_send_digit), '4');
    g_signal_connect(btn_digit5, "clicked", G_CALLBACK(gui_send_digit), '5');
    g_signal_connect(btn_digit6, "clicked", G_CALLBACK(gui_send_digit), '6');
    g_signal_connect(btn_digit7, "clicked", G_CALLBACK(gui_send_digit), '7');
    g_signal_connect(btn_digit8, "clicked", G_CALLBACK(gui_send_digit), '8');
    g_signal_connect(btn_digit9, "clicked", G_CALLBACK(gui_send_digit), '9');
    g_signal_connect(btn_thousand000, "clicked", G_CALLBACK(gui_send_three_zeros), NULL);
    g_signal_connect(btn_exp, "clicked", G_CALLBACK(gui_insert_exp), NULL);
    g_signal_connect(btn_fl_point, "clicked", G_CALLBACK(gui_insert_comma), NULL);
    g_signal_connect(btn_neg, "clicked", G_CALLBACK(gui_negate), NULL);
    g_signal_connect(btn_add, "clicked", G_CALLBACK(gui_select_bi_op), ADD);
    g_signal_connect(btn_sub, "clicked", G_CALLBACK(gui_select_bi_op), SUB);
    g_signal_connect(btn_mul, "clicked", G_CALLBACK(gui_select_bi_op), MUL);
    g_signal_connect(btn_div, "clicked", G_CALLBACK(gui_select_bi_op), DIV);
    g_signal_connect(btn_rtn, "clicked", G_CALLBACK(gui_evaluate), NULL);
    g_signal_connect(btn_pow, "clicked", G_CALLBACK(gui_select_bi_op), POW);
    g_signal_connect(btn_pow2, "clicked", G_CALLBACK(gui_power_to), 2);
    g_signal_connect(btn_pow3, "clicked", G_CALLBACK(gui_power_to), 3);
    g_signal_connect(btn_root, "clicked", G_CALLBACK(gui_select_bi_op), ROOT);
    g_signal_connect(btn_root_square, "clicked", G_CALLBACK(gui_root_of), 2);
    g_signal_connect(btn_root_cubic, "clicked", G_CALLBACK(gui_root_of), 3);
    g_signal_connect(btn_fact, "clicked", G_CALLBACK(gui_eval_un_op), FACT);
    g_signal_connect(btn_combination, "clicked", G_CALLBACK(gui_select_bi_op), COMBINATIONAL);

    // Keyboard mapping
    GtkEventController *key_controller = gtk_event_controller_key_new();
    g_signal_connect(key_controller, "key-pressed", G_CALLBACK(key_press_cb), NULL);
    gtk_widget_add_controller(window, key_controller);

    // Interactive help: shows the text on the help label when mouse cursor is hovering over a certain button
    connect_hover_action_to_help_change(btn_cancel, "[C] Cancel, resets the calculator");
    connect_hover_action_to_help_change(btn_backspace, "[Backspace] Undos the last modification");
    connect_hover_action_to_help_change(btn_digit0, "[0] Inserts the digit 0");
    connect_hover_action_to_help_change(btn_digit1, "[1] Inserts the digit 1");
    connect_hover_action_to_help_change(btn_digit2, "[2] Inserts the digit 2");
    connect_hover_action_to_help_change(btn_digit3, "[3] Inserts the digit 3");
    connect_hover_action_to_help_change(btn_digit4, "[4] Inserts the digit 4");
    connect_hover_action_to_help_change(btn_digit5, "[5] Inserts the digit 5");
    connect_hover_action_to_help_change(btn_digit6, "[6] Inserts the digit 6");
    connect_hover_action_to_help_change(btn_digit7, "[7] Inserts the digit 7");
    connect_hover_action_to_help_change(btn_digit8, "[8] Inserts the digit 8");
    connect_hover_action_to_help_change(btn_digit9, "[9] Inserts the digit 9");
    connect_hover_action_to_help_change(btn_thousand000, "Inserts three zeros");
    connect_hover_action_to_help_change(btn_exp, "[E] Lets you insert exponent");
    connect_hover_action_to_help_change(btn_fl_point, "[dot][comma] Inserts decimal point");
    connect_hover_action_to_help_change(btn_neg, "[N] Negates the current number");
    connect_hover_action_to_help_change(btn_add, "[+] Sum");
    connect_hover_action_to_help_change(btn_sub, "[-] Difference");
    connect_hover_action_to_help_change(btn_mul, "[*] Mulitplication");
    connect_hover_action_to_help_change(btn_div, "[/] Division");
    connect_hover_action_to_help_change(btn_rtn, "[Enter] Evaluetes the operation");
    connect_hover_action_to_help_change(btn_pow, "[P] N-th power");
    connect_hover_action_to_help_change(btn_pow2, "Powers the number to the second");
    connect_hover_action_to_help_change(btn_pow3, "Powers the number to the third");
    connect_hover_action_to_help_change(btn_root, "[R] N-th root (Index of the root is 2nd operand)");
    connect_hover_action_to_help_change(btn_root_square, "Square root");
    connect_hover_action_to_help_change(btn_root_cubic, "Third root of");
    connect_hover_action_to_help_change(btn_fact, "[F] Factorial");
    connect_hover_action_to_help_change(btn_combination, "[B] Combination number");

    // Prevents the buttons from getting focus when clicked with mouse.
    // Otherwise the buttons will take focus and keyboard key Enter will execute the focused button. 
    gtk_widget_set_focusable(btn_cancel, FALSE);
    gtk_widget_set_focusable(btn_backspace, FALSE);
    gtk_widget_set_focusable(btn_digit0, FALSE);
    gtk_widget_set_focusable(btn_digit1, FALSE);
    gtk_widget_set_focusable(btn_digit2, FALSE);
    gtk_widget_set_focusable(btn_digit3, FALSE);
    gtk_widget_set_focusable(btn_digit4, FALSE);
    gtk_widget_set_focusable(btn_digit5, FALSE);
    gtk_widget_set_focusable(btn_digit6, FALSE);
    gtk_widget_set_focusable(btn_digit7, FALSE);
    gtk_widget_set_focusable(btn_digit8, FALSE);
    gtk_widget_set_focusable(btn_digit9, FALSE);
    gtk_widget_set_focusable(btn_thousand000, FALSE);
    gtk_widget_set_focusable(btn_exp, FALSE);
    gtk_widget_set_focusable(btn_fl_point, FALSE);
    gtk_widget_set_focusable(btn_neg, FALSE);
    gtk_widget_set_focusable(btn_add, FALSE);
    gtk_widget_set_focusable(btn_sub, FALSE);
    gtk_widget_set_focusable(btn_mul, FALSE);
    gtk_widget_set_focusable(btn_div, FALSE);
    gtk_widget_set_focusable(btn_rtn, FALSE);
    gtk_widget_set_focusable(btn_pow, FALSE);
    gtk_widget_set_focusable(btn_pow2, FALSE);
    gtk_widget_set_focusable(btn_pow3, FALSE);
    gtk_widget_set_focusable(btn_root, FALSE);
    gtk_widget_set_focusable(btn_root_square, FALSE);
    gtk_widget_set_focusable(btn_root_cubic, FALSE);
    gtk_widget_set_focusable(btn_fact, FALSE);
    gtk_widget_set_focusable(btn_combination, FALSE);

    change_text_in_display("0");

    gtk_widget_show(window);
}

static void gui_cancel(GtkWidget *widget, gpointer data)
{
    change_text_based_on_result(caleng_cancel(CALC_ENGINE));
}

static void gui_backspace(GtkWidget *widget, gpointer data)
{
    change_text_based_on_result(caleng_backspace(CALC_ENGINE));
}

static void gui_send_digit(GtkWidget *widget, char digit)
{
    change_text_based_on_result(caleng_send_digit(CALC_ENGINE, digit));
}

static void gui_send_three_zeros(GtkWidget *widget, gpointer data)
{
    caleng_send_digit(CALC_ENGINE, '0');
    caleng_send_digit(CALC_ENGINE, '0');
    change_text_based_on_result(caleng_send_digit(CALC_ENGINE, '0'));
}

static void gui_insert_exp(GtkWidget *widget, gpointer data)
{
    change_text_based_on_result(caleng_insert_exp(CALC_ENGINE));
}

static void gui_insert_comma(GtkWidget *widget, gpointer data)
{
    change_text_based_on_result(caleng_insert_decimal_point(CALC_ENGINE));
}

static void gui_negate(GtkWidget *widget, gpointer data)
{
    change_text_based_on_result(caleng_negate(CALC_ENGINE));
}

static void gui_evaluate(GtkWidget *widget, gpointer data)
{
    change_text_based_on_result(caleng_evaluate(CALC_ENGINE));
}

static void gui_eval_un_op(GtkWidget *widget, int un_op)
{
    change_text_based_on_result(caleng_eval_un_op(CALC_ENGINE, un_op));
}

static void gui_select_bi_op(GtkWidget *widget, int bi_op)
{
    change_text_based_on_result(caleng_select_bi_op(CALC_ENGINE, bi_op));
}

static void gui_power_to(GtkWidget *widget, int value)
{
    caleng_select_bi_op(CALC_ENGINE, POW);
    caleng_send_digit(CALC_ENGINE, value + '0');
    change_text_based_on_result(caleng_evaluate(CALC_ENGINE));
}

static void gui_root_of(GtkWidget *widget, int value)
{
    caleng_select_bi_op(CALC_ENGINE, ROOT);
    caleng_send_digit(CALC_ENGINE, value + '0');
    change_text_based_on_result(caleng_evaluate(CALC_ENGINE));
}

void change_text_in_display(char *string)
{
    char *markup_text = "<big><span font='22' line_height='1/1000' >%s</span></big>";
    char final_text[1000];
    snprintf(final_text, 1000, markup_text, string);
    gtk_label_set_markup(GTK_LABEL(GLOBAL_DISPLAY), final_text);
}

void change_text_based_on_result(result_t result)
{
    #ifdef DEBUG
    // debugging info
    fprintf(stderr, "[%d]: %s\tbuffer: %s\n", result.rtn_code, result.to_display, CALC_ENGINE->input_buffer);
    #endif
    char text[BUFFER_SIZE + 1];
    switch (result.rtn_code)
    {
    case OK:
        strcpy(text, result.to_display);
        break;
    case OVERFLOW_ERR:
        strcpy(text, "Overflow Error");
        break;
    case SYNTAX_ERR:
        strcpy(text, "Syntax Error");
        break;
    case MATH_ERR:
        strcpy(text, "Math Error");
        break;
    default:
        fprintf(stderr, "WARNING: change_text_based_on_result - neznámy návratový kód result\n");
        break;
    }
    change_text_in_display(text);
}

static gboolean key_press_cb(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer data)
{
    if (keyval == GDK_KEY_C || keyval == GDK_KEY_c)
    {
        gui_cancel(NULL, NULL);
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_BackSpace)
    {
        gui_backspace(NULL, NULL);
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_KP_0 || keyval == GDK_KEY_0)
    {
        gui_send_digit(NULL, '0');
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_KP_1 || keyval == GDK_KEY_1)
    {
        gui_send_digit(NULL, '1');
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_KP_2 || keyval == GDK_KEY_2)
    {
        gui_send_digit(NULL, '2');
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_KP_3 || keyval == GDK_KEY_3)
    {
        gui_send_digit(NULL, '3');
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_KP_4 || keyval == GDK_KEY_4)
    {
        gui_send_digit(NULL, '4');
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_KP_5 || keyval == GDK_KEY_5)
    {
        gui_send_digit(NULL, '5');
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_KP_6 || keyval == GDK_KEY_6)
    {
        gui_send_digit(NULL, '6');
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_KP_7 || keyval == GDK_KEY_7)
    {
        gui_send_digit(NULL, '7');
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_KP_8 || keyval == GDK_KEY_8)
    {
        gui_send_digit(NULL, '8');
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_KP_9 || keyval == GDK_KEY_9)
    {
        gui_send_digit(NULL, '9');
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_E || keyval == GDK_KEY_e)
    {
        gui_insert_exp(NULL, NULL);
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_KP_Separator || keyval == GDK_KEY_KP_Decimal || keyval == GDK_KEY_comma || keyval == GDK_KEY_period)
    {
        gui_insert_comma(NULL, NULL);
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_N || keyval == GDK_KEY_n)
    {
        gui_negate(NULL, NULL);
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_Return || keyval == GDK_KEY_KP_Enter)
    {
        gui_evaluate(NULL, NULL);
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_F || keyval == GDK_KEY_f)
    {
        gui_eval_un_op(NULL, FACT);
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_plus || keyval == GDK_KEY_KP_Add)
    {
        gui_select_bi_op(NULL, ADD);
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_minus || keyval == GDK_KEY_KP_Subtract)
    {
        gui_select_bi_op(NULL, SUB);
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_asterisk || keyval == GDK_KEY_KP_Multiply)
    {
        gui_select_bi_op(NULL, MUL);
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_slash || keyval == GDK_KEY_KP_Divide)
    {
        gui_select_bi_op(NULL, DIV);
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_P || keyval == GDK_KEY_p)
    {
        gui_select_bi_op(NULL, POW);
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_R || keyval == GDK_KEY_r)
    {
        gui_select_bi_op(NULL, ROOT);
        return GDK_EVENT_STOP;
    }
    if (keyval == GDK_KEY_B || keyval == GDK_KEY_b)
    {
        gui_select_bi_op(NULL, COMBINATIONAL);
        return GDK_EVENT_STOP;
    }
    return GDK_EVENT_PROPAGATE;
}

static void change_help_description(GtkEventController *controller, gpointer data)
{
    gtk_label_set_label(GTK_LABEL(GLOBAL_HELP_LABEL), data);
}
