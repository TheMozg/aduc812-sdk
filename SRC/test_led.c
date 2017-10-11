#include "aduc812.h"
#include "led.h"
#include "max.h"

// Define triggering value on DIP switches
#define DIP_INPUT 0xAA

// Delay between LED toggle 300 cycles
#define DELAY_TIME 300

// Waiting in a loop doing nothing
void delay( unsigned long ms ) {

	volatile unsigned long i, j;

    for( j = 0; j < ms; j++ )
    {
        for( i = 0; i < 50; i++ );
    }
}

void main( void ) {

	// Starting LED
	char i = 6;

	// Variable for DIP input
	unsigned char input;

	// Doing in infinite loop
	while(1) {
  	// Reading DIP switches value
		input = read_max(EXT_LO);

  	// Disabling all LEDs
		leds( 0x00 );

  	// If correct is passed, then run the animation, else show inverterted value
		if ( input == DIP_INPUT ) {
			
    	// If last LED then light up first
			if( i < 0 ) {
				i = 7;
			}		
		
    	// Set LED up
			led( i, 1 );
		
    	// Light up second LED according to animation
			if( i + 1 == 8 ) {
				led( 0, 1 );
			} else {
				led( i + 1, 1 );
			}

    	// Going from LED 7 to 0
			i--;
		} else { 
			leds( input ); 
		}

  	// Wait a bit so we could actually see the LEDs
		delay( DELAY_TIME );
	}
}    
