#include "app_echo.h"
#include "uart_blocking.h"

void run_echo_app(int init_app) {
    unsigned char c;
    if (init_app) {
        uart_init_blocking();
    }
    c = uart_read_blocking();
    if( c >= '0' && c <= '9' ) {
        for( c; c >= '0'; c-- ) {
            uart_write_blocking( c );
        }
        uart_write_blocking( '\r' );
        uart_write_blocking( '\n' );
    }
}