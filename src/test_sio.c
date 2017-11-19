#include <stdio.h>
#include "aduc812.h"
#include "sio.h"
#include "led.h"
#include "dip.h"
#include "test_sio.h"
#include "fifo.h"

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
  unsigned char r = 0;
  unsigned char i;
  unsigned char d;
  fifo_t* fifo = { NULL, NULL };
  int f = 1;
  init_sio( BAUD );

  while( 1 ) {
   // leds( get_dips( ) );
    if( rsiostat() ) {
      c = rsio();
      
      enqueue( fifo, 'd' );
      enqueue( fifo, 'i' );
      enqueue( fifo, 'c' );
      enqueue( fifo, 'k' );
      enqueue( fifo, 's' );

      for( i = 0; i < 5; i++ ) {
        d = dequeue( fifo );
        wsio( d );
      }
      type( EOL );
      
     // fifo_deinit( fifo );

    //  desc_digs( c );
    }

  }
}  
