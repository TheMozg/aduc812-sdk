#include <stdio.h>
#include "aduc812.h"
#include "sio.h"
#include "led.h"
#include "dip.h"
#include "test_sio.h"
#include "fifo.h"

enum mode g_mode = ASYNC;

// Task 1
void desc_digs( unsigned char c ) {

  if( c >= ascii_zero && c <= ascii_nine ) {
    for( c; c >= ascii_zero; c-- ) {
      wsio( c );
    }
  }
  type( EOL );

}


void main( void ) {

  unsigned char c;
  /*unsigned char i;
  unsigned char d;
  fifo_t* fifo = 0;
  int s;
  fifo_init( fifo );*/
  
  init_sio( BAUD );
  /*EA = 0;
  ES = 0;*/
  EA = 1;
  while( 1 ) {
    
    if( rsiostat( ) ) {
     // c = rsio();
    //  wsio( c );
      type( EOL );
    }

  }
}  
