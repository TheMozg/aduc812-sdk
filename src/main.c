#include "notes.h"
#include "sys_timer.h"
#include "isr.h"
#include "sound.h"

#define DELAY_MS 500

void main( void ) {
  init_timers( );
  set_vectors( );
  
  while( 1 ) {
    set_note( C_H, C_L );
    delay_ms( DELAY_MS );
    set_note( D_H, D_L );
    delay_ms( DELAY_MS );
    set_note( E_H, E_L );
    delay_ms( DELAY_MS );
    set_note( F_H, F_L );
    delay_ms( DELAY_MS );
    set_note( G_H, G_L );
    delay_ms( DELAY_MS );
    set_note( A_H, A_L );
    delay_ms( DELAY_MS );
    set_note( B_H, B_L );
    delay_ms( DELAY_MS );
  }
}
