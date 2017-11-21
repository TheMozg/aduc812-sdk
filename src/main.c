#include "sys_timer.h"
#include "isr.h"
#include "sound.h"
#include "notes.h"
#include "MasterOfPuppets.h"
#define DELAY_MS 500

void main( void ) {
  init_timers( );
  set_vectors( );
  
  while( 1 ) {

    set_note( B_NOTE_4 );
    delay_ms( DELAY_MS );

    set_note( A_NOTE_4 );
    delay_ms( DELAY_MS );

    set_note( G_NOTE_4 );
    delay_ms( DELAY_MS );

    set_note( F_NOTE_4  );
    delay_ms( DELAY_MS );

    set_note( E_NOTE_4 );
    delay_ms( DELAY_MS );

    set_note( D_NOTE_4 );
    delay_ms( DELAY_MS );

    set_note( C_NOTE_4 );
    delay_ms( DELAY_MS );

  }
}

