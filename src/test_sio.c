#include <stdio.h>
#include "aduc812.h"
#include "sio.h"
#include "led.h"
#include "dip.h"
#include "test_sio.h"
#include "fifo.h"

enum mode g_mode = ASYNC;

void desc_digs( unsigned char c, fifo_t* fifo ) {

  if( c >= '0' && c <= '9' ) {
    for( c; c >= '0'; c-- ) {
      //wsio( c );
      enqueue( fifo, c );
    }
  }
  //type( EOL );
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
    leds( get_dips( ) );

    if( rsiostat( ) ) {
      c = rsio( );

      desc_digs( c, fifo );

      while( fifo_get_size( fifo ) > 0 ) {
        d = dequeue( fifo );
        wsio( c );
      
      }
      type( EOL );
      
     // fifo_deinit( fifo );

    //  desc_digs( c );
    }
  }
}  
