#include "aduc812.h"
#include "sio.h"
#include "fifo.h"
#include "isr_utils.h"
#include "led.h"

fifo_t* r_fifo;
fifo_t* t_fifo;

unsigned char s = 0x81;
void SIO_ISR( void ) __interrupt ( 4 ) {
  
  if( TI ) {
    TI = 0;
    leds( s );
    if( fifo_get_size( t_fifo ) > 0 ) SBUF = dequeue( t_fifo );

    leds( 0x0F );
  }

  if( RI ) {
   
    leds( 0xAA );
    enqueue( r_fifo, SBUF );
    RI = 0;
  }

  leds( 0 );
}

void init_sio( unsigned char speed ) {

  fifo_init( r_fifo );
  fifo_init( t_fifo );

  set_vector(  0x2023, (void *)SIO_ISR );

  TH1     =  speed; 
  TMOD   |=  0x20; //Таймер 1 будет работать в режиме autoreload
  TCON   |=  0x40; //Запуск таймера 1
  SCON    =  0x50; //Настройки последовательного канала
  ES    =  1;  //Разрешение прерываний от приемопередатчика
  EA    =  0;  
}

unsigned char rsiostat( void ) {
  if( g_mode == SYNC ) return RI;
  if( g_mode == ASYNC ) return ( fifo_get_size( r_fifo ) > 0 ) ? 1 : 0;
  return 0;
}

void wsio( unsigned char c ) {
  char f = 0;
  /*if( g_mode == SYNC ) {
    SBUF = c;
    TI   = 0;
  }*/

  if( g_mode == ASYNC ) {
   // char oES = ES;
    ES = 0;
    if( fifo_get_size( t_fifo ) == 0 ) f = 1;
    enqueue( t_fifo, c );
    
    if( f ) TI = 1;

    ES = 1;
  }
//  while( !TI );
}

unsigned char rsio( void ) {
  unsigned char c = 0;

 /* while( !RI );
  if( g_mode == SYNC ) {
    RI = 0;
    return SBUF;
  } */

  if( g_mode == ASYNC ) {
  //  char oES = ES;
    ES = 0;

    if( fifo_get_size( r_fifo ) > 0 ) c = dequeue( r_fifo );
    //RI = 1;
    
    ES = 1;
    return c;
  }

  return 0;
}

void type( char * str ) {
  char f = 0;
  /*if( g_mode == SYNC ) {
    while( *str ) wsio( *str++ );
  }*/

  if( g_mode == ASYNC ) {
  //  char oES = ES;
    ES = 0;
    if( fifo_get_size( t_fifo ) == 0 ) f = 1;
    
    while( *str ) enqueue( t_fifo, *str++ );
    
    if( f ) TI = 1;
    ES = 1;
  }
}

