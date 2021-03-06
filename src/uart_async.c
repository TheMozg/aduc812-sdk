#include "uart_async.h"
#include "uart_blocking.h"
#include "isr_utils.h"
#include "fifo.h"
#include "aduc812.h"

static char read_queue[FIFO_SIZE];
static char write_queue[FIFO_SIZE];

void SIO_ISR() __interrupt {
    ES = 0;
    if(TI) {
        TI = 0;
        // remove transmitted byte from queue
        fifo_dequeue( write_queue );
        // transmit next byte in queue, if any
        if( fifo_get_size( write_queue ) > 0 ){
            // start transmitting byte
            // but keep it in queue until transmission is complete
            SBUF = fifo_peek( write_queue );
        }
    }

    if(RI) {
        RI = 0;
        fifo_enqueue( read_queue, SBUF );
    }
    ES = 1;
}

void uart_init_async() {
    fifo_init( read_queue );               // init read queue
    fifo_init( write_queue );              // init write queue
    uart_init_blocking();                  // init core uart features
    set_vector( 0x2023, (void *)SIO_ISR ); // register interrupt handler
    EA = 1;                                // enable interrupts
    ES = 1;                                // enable uart interrupt
}

void uart_write_async( char c ) {
    int first_write = 0;
    ES = 0;
    if( fifo_get_size( write_queue ) == 0 ) {
        // enqueue dummy byte which will be removed
        fifo_enqueue( write_queue, '?' );
        first_write = 1;
    }
    fifo_enqueue( write_queue, c );
    ES = 1;
    if( first_write == 1 ) {
        TI = 1; // manually initiate transmission
    }
}

void uart_write_str_async( char * str ) {
    while( *str ) uart_write_async( *str++ );
}

char uart_read_async() {
    char res = '\0';
    ES = 0;
    if( fifo_get_size( read_queue ) != 0 ) {
        res = fifo_dequeue( read_queue );
    }
    ES = 1;
    return res;
}
