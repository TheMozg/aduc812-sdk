#pragma once

// Define main clock speed
#define MCLKIN 11059200

// Calculate timer ticks to count a millisecond
#define MS 0xFFFF - ( MCLKIN / 12 ) / 1000

// Break ms into two 8 bit values
#define MS_H ( MS >> 8 ) & 0x00FF
#define MS_L MS & 0x00FF

// Time in ms since timer1 start
extern unsigned long systime;

// Get current systime
unsigned long get_ms_counter( void ); 

// Get time difference
unsigned long d_time_ms( unsigned long t2 );

// Set delay in ms
void delay_ms( unsigned long ms ); 

// Init timers
void init_timers( );
