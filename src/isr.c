#include "isr_utils.h"
#include "isr.h"
#include "sys_timer.h"
#include "aduc812.h"
#include "max.h"
#include "sound.h"
#include "led.h"

// INT1 counter
unsigned char k;

// Toggler for ENA in T0 ISR
char t;

char g_note_h;
char g_note_l;

unsigned long systime = 0;

// Handler for INT1 interrupt, controls LED output
void INT1_ISR( void ) __interrupt {
	k++;
	leds( k );
}

// Handler for timer1 interrupts, interrupts every millisecond
void T1_ISR ( void ) __interrupt {
  systime++;
    
  TH1 = MS_H;
  TL1 = MS_L;
}

// Handler for timer0 interrupts, generates sound of set frequency
void T0_ISR( void ) __interrupt {

  if( t ) {
    write_max(ENA, VOL1);
    t = 0;
  } else {
    write_max(ENA, VOL0);
    t = 1;
  }
  TH0 = g_note_h;
  TL0 = g_note_l;
}

void set_vectors( ) {
  set_vector( 0x200B, (void *)T0_ISR );
  set_vector( 0x2013, (void *)INT1_ISR );
  set_vector( 0x201B, (void *)T1_ISR );
}
