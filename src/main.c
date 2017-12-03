#include "aduc812.h"
#include "uart_blocking.h"
#include "uart_async.h"
#include "led.h"
#include "dip.h"

typedef enum {
    none,
    blocking,
    async
} uart_driver_t;

uart_driver_t get_driver() {
    leds(read_dip());
    if (read_dip() == 0xFF)
        return async;
    return blocking;
}

void main() {
    uart_driver_t last_driver = none;
    while( 1 ) {
        uart_driver_t driver = get_driver();
        unsigned char c;

        if (driver == blocking) {
            if (driver != last_driver)
                uart_init_blocking();
            c = uart_read_blocking();
            if( c >= '0' && c <= '9' ) {
                for( c; c >= '0'; c-- ) {
                    uart_write_blocking( c );
                }
                uart_write_blocking( '\r' );
                uart_write_blocking( '\n' );
            }
        }

        if (driver == async) {
            if (driver != last_driver)
                uart_init_async();
            c = uart_read_async();
            if( c >= '0' && c <= '9' ) {
                for( c; c >= '0'; c-- ) {
                    uart_write_async( c );
                }
                uart_write_async( '\r' );
                uart_write_async( '\n' );
            }
        }

        last_driver = driver;

    }
}