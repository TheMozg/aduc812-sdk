#pragma once

// Define main clock speed
#define MCLKIN 11059200

// Calculate timer ticks to count a millisecond
#define MS 0xFFFF - ( MCLKIN / 12 ) / 1000

// Break ms into two 8 bit values
#define MS_H ( MS >> 8 ) & 0x00FF
#define MS_L MS & 0x00FF

extern unsigned long systime;

unsigned long get_ms_counter( void ); 

unsigned long d_time_ms( unsigned long t2 );

void delay_ms( unsigned long ms ); 

void init_timers( );
