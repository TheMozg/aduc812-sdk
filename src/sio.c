#include "aduc812.h"
#include "sio.h"
#include "fifo.h"
#include "isr_utils.h"
#include "led.h"

fifo_t* r_fifo;
fifo_t* t_fifo;

const unsigned char dick = 'D';
void SIO_ISR( void ) __interrupt ( 4 ) {
  
  if( TI ) {
    TI = 0;
    //if( fifo_get_size( t_fifo ) > 0 ) SBUF = dequeue( t_fifo );
    if( fifo_get_size( t_fifo ) > 0 ) SBUF = dick;

    leds( 0x0F );
  }

  if( RI ) {
    //enqueue( r_fifo, SBUF );
    enqueue( r_fifo, dick );
    RI = 0;

    leds( 0xAA );
  }

  //leds( 0 );
}

void init_sio( unsigned char speed ) {
  fifo_init( r_fifo );
  fifo_init( t_fifo );

  set_vector(  0x2023, (void *)SIO_ISR );
    
  TH1       =  speed; 
  TMOD     |=  0x20; //Таймер 1 будет работать в режиме autoreload
  TCON     |=  0x40; //Запуск таймера 1
  SCON      =  0x50; //Настройки последовательного канала
  ES        =  0;    //Запрещение прерываний от приемопередатчика
  //EA        =  1;
}

unsigned char rsiostat( void ) {
  return RI;
 // return ( fifo_get_size( r_fifo ) > 0 ) ? 1 : 0;
}

void wsio( unsigned char c ) {
  /*ES = 0;
    //if( fifo_get_size( t_fifo ) == 0 ) f = 1;
  enqueue( t_fifo, c );
    
  if( fifo_get_size( t_fifo ) > 0 ) TI = 1;

  ES = 1;*/
  SBUF = c;
  TI   = 0;
  while( !TI );
}

unsigned char rsio( void ) {
    while( !RI );
    RI = 0;
    return SBUF;
  /*unsigned char c;
  ES = 0;

  if( fifo_get_size( r_fifo ) > 0 ) c = dequeue( r_fifo );
    //RI = 1;
    
  ES = 1;

  return c;*/
}

void type( char * str ) {
  while( *str ) wsio( *str++ );
  /*ES = 0;
  
  while( *str ) enqueue( t_fifo, *str++ );

  if( fifo_get_size( t_fifo ) > 0 ) TI = 1;

  ES = 1;*/
}

