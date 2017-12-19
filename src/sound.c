#include "sound.h"
#include "max.h"
#include "aduc812.h"
#include "isr_utils.h"

#define DIAPHRAGM_MIN 0
#define DIAPHRAGM_MAX 0b00011100

static int diaphragm_is_min;

void move_diaphragm(unsigned char level) {
    write_max(ENA, level);
}

void T2_ISR() __interrupt {
    if( diaphragm_is_min ) {
        move_diaphragm(DIAPHRAGM_MAX);
        diaphragm_is_min = 0;
    } else {
        move_diaphragm(DIAPHRAGM_MIN);
        diaphragm_is_min = 1;
    }
    TF2 = 0;
}

void sound_set_note( int hz ) {
    unsigned short autoreload = 0xFFFF - MCLKIN / ( hz * 2 * 12 );
    RCAP2H = (autoreload >> 8 ) & 0xFF;
    RCAP2L = (autoreload & 0xFF);
}

void sound_init(){
    diaphragm_is_min = 0;
    set_vector( 0x202B, (void *)T2_ISR ); // register interrupt handler
    EA     = 1;                           // enable interrupts
    ET2    = 1;                           // enable timer 2 interrupt
    PT2    = 1;                           // set timer 2 high priority
    T2CON  = 0x00;                        // reset timer 2 settings
    RCAP2L = 0;                           // reset autoreload registers
    RCAP2H = 0;                           // reset autoreload registers
}

void sound_enable() {
    TR2    = 1;                           // enable timer 2
}

void sound_disable() {
    TR2    = 0;                           // disable timer 2
}
