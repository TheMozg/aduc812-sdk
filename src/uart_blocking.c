#include "uart_blocking.h"
#include "aduc812.h"

void uart_init_blocking() {
    TMOD = (TMOD & 0x0F) | 0x20; // set timer 1 to mode 2 (8 bit autoreload)
    PCON = PCON & 0x7F; // disable double UART baud rate boost (2400 baud rate)
    SCON = 0x50; // set UART to mode 1 and enable reception
    ES   = 0;    // disable uart interrupt
    ET1  = 0;    // disable timer 1 interrupt
    TH1  = 0xF4; // autorealod timer 1 with preset value (2400 baud rate)
    TR1  = 1;    // enable timer 1
}

void uart_write_blocking( unsigned char c ) {
    TI = 0;
    SBUF = c;
    while( !TI );
}

unsigned char uart_read_blocking() {
    RI = 0;
    while( !RI );
    return SBUF;
}

