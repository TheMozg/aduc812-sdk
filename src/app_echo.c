#include "app_echo.h"
#include "uart_async.h"
#include "keypad.h"

void init_echo_app() {
    uart_init_async();
    keypad_init();
}

void run_echo_app() {
    char c = keypad_read();
    if( c ) {
       uart_write_async( c );
       uart_write_str_async( "\r\n" );
    }
}
