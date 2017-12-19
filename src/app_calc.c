#include "app_calc.h"
#include "uart_async.h"
#include "keypad.h"
#include "util.h"

typedef enum {
    init,
    read_dividend,
    read_divisor,
    print_result,
    print_error
} calc_state_t;

static calc_state_t calc_state;
static unsigned char length;
static int divident;
static int divisor;

void init_calc_app() {
    uart_init_async();
    keypad_init();
    calc_state = init;
}

void run_calc_app() {
    char c = keypad_read();
    if ( c ) uart_write_async(c);
    switch (calc_state) {
        case init:
            calc_state = read_dividend;
            length = 0;
            divident = 0;
            divisor = 0;
            break;
        case read_dividend:
            if ( c >= '0' && c <= '9' ) {
                if (length == 2) {
                    calc_state = print_error;
                }
                if (length == 1) {
                    divident *= 10;
                }
                divident += c - '0';
                length++;
                break;
            }
            if ( c == '/' ) {
                if (length == 0){
                    calc_state = print_error;
                }
                else {
                    length = 0;
                    calc_state = read_divisor;
                }
                break;
            }
            if ( c == 0 ) {
                break;
            }
            calc_state = print_error;
            break;
        case read_divisor:
            if ( c >= '0' && c <= '9' ) {
                if (length == 2) {
                    calc_state = print_error;
                }
                if (length == 1) {
                    divisor *= 10;
                }
                divisor += c - '0';
                length++;
                break;
            }
            if ( c == '=' ) {
                if (length == 0)
                    calc_state = print_error;
                else
                    calc_state = print_result;
                break;
            }
            if ( c == 0 ) {
                break;
            }
            calc_state = print_error;
            break;
        case print_result:
            if (divisor == 0){
                uart_write_str_async("\r\nCannot divide by 0!\r\n");
            } else {
                char str[3];
                itoa(divident / divisor, str);
                uart_write_str_async(str);
                uart_write_str_async("\r\n");
            }
            calc_state = init;
            break;
        case print_error:
            uart_write_str_async("\r\nInput error!\r\n");
            calc_state = init;
            break;
    }
}
