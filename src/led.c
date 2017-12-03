#include "aduc812.h"
#include "max.h"
#include "led.h"

static unsigned char old_led = 0;

void led( unsigned char n, unsigned char on )
{
    unsigned char c;
    unsigned char mask = 1;

    if( n > 7 ) return;

    c = old_led;

    mask <<= n;

    if( on )
        c |= mask;
    else
        c &= ~mask;         

    write_max( SV, c );     

    old_led = c;
}

void leds( unsigned char on )
{
    write_max( SV, on );     

    old_led = on;
}



