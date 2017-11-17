#include "sys_timer.h"
#include "notes.h"
#include "aduc812.h"

unsigned long get_ms_counter( void ) { 
  unsigned long res; 
  ET1 = 0;
  res = systime; 
  ET1 = 1;
  return res; 
} 

unsigned long d_time_ms( unsigned long t2 ) { 
  unsigned long t1 = get_ms_counter(); 
  return t1 - t2;   
} 

void delay_ms( unsigned long ms ) { 
  unsigned long t1 = get_ms_counter(); 
  while ( 1 ) { 
    if ( d_time_ms( t1 ) > ms ) break; 
  }   
} 

void init_timers( ) {
  TMOD = 0x11;
  
  IT1 = 1;
  
  PT1 = 0;
  PT0 = 1;
  
  TH0 = A_H;
  TL0 = A_L;
  
  TH1 = MS_H;
  TL1 = MS_L;
 
  TR1 = 1;
  TR0 = 1;

  ET1 = 1;
  ET0 = 1;
  
  EX1 = 1;
  
  EA = 1;
}
