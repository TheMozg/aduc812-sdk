#include "keypad.h"
#include "aduc812.h"
#include "isr_utils.h"
#include "max.h"
#include "fifo.h"
#include "sound.h"
#include "notes.h"

#define SCAN_FREQ 100
#define SAMPLE_SIZE 10
#define REPEAT_KEY_MS 1000

static char read_queue[FIFO_SIZE];

__code static char key_chars[] = "123A"
                                 "456B"
                                 "789C"
                                 "*0=/";

__code static short key_tones[] = {
    C_NOTE_3, D_NOTE_3, F_NOTE_3, G_NOTE_3,
    C_SHARP_NOTE_3, D_SHARP_NOTE_3, F_SHARP_NOTE_3, G_SHARP_NOTE_3,
    C_NOTE_4, D_NOTE_4, F_NOTE_4, G_NOTE_4,
    C_SHARP_NOTE_4, D_SHARP_NOTE_4, F_SHARP_NOTE_4, G_SHARP_NOTE_4
};

static unsigned char key_pressed_samples[16];
static unsigned char key_is_pressed[16];
__pdata static unsigned short key_is_pressed_ms[16];

static unsigned char autoreload_h;
static unsigned char autoreload_l;

unsigned char scan_column(unsigned char column) {
    // set column as 0, for example 11110111
    write_max(0x0, ~ (0b1 << column));
    // return selected rows as bit array (lower 4 bits)
    return ~(read_max(0x0) >> 4);
}

void T0_ISR() __interrupt {
    unsigned int key_is_pressed_raw[16];
    unsigned int i, j, key_pressed_count;
    ET0 = 0;

    for (i = 0; i < 4; i++) {
        unsigned char rows = scan_column(i);
        for (j = 0; j < 4; j++) {
            key_is_pressed_raw[j*4+i] = (rows >> j) & 0x01;
        }
    }

    key_pressed_count = 0;
    for (i = 0; i < 16; i++) {
        if (key_is_pressed_raw[i] == 1) {
            if (key_pressed_samples[i] < SAMPLE_SIZE)  key_pressed_samples[i] += 1;
        } else {
            if (key_pressed_samples[i] > 0)            key_pressed_samples[i] -= 1;
        }
        if (key_is_pressed[i]) {
            key_is_pressed_ms[i] += 1000 / SCAN_FREQ;
            key_pressed_count++;
            if (key_pressed_samples[i] == 0)           key_is_pressed[i] = 0;
        } else {
            if (key_pressed_samples[i] == SAMPLE_SIZE) key_is_pressed[i] = 1;
            key_is_pressed_ms[i] = REPEAT_KEY_MS;
        }
    }

    if (key_pressed_count == 1 || key_pressed_count == 2) {
        sound_enable();
        for (i = 0; i < 16; i++) {
            if (key_is_pressed[i] && key_is_pressed_ms[i] >= REPEAT_KEY_MS) {
                fifo_enqueue(read_queue, key_chars[i]);
                sound_set_note(key_tones[i]);
                key_is_pressed_ms[i] = 0;
            }
        }
    } else {
        sound_disable();
    }

    TH0 = autoreload_h;
    TL0 = autoreload_l;
    ET0 = 1;
}

void keypad_init() {
    unsigned int i;
    unsigned short autoreload = 0xFFFF - MCLKIN / ( SCAN_FREQ * 12 );
    autoreload_h = (autoreload >> 8 ) & 0xFF;
    autoreload_l = (autoreload & 0xFF);
    for (i = 0; i < 16; i++) {
        key_is_pressed[i] = 0;
        key_is_pressed_ms[i] = 0;
        key_pressed_samples[i] = 0;
    }
    sound_init();
    fifo_init( read_queue );              // init read queue
    set_vector( 0x200B, (void *)T0_ISR ); // register interrupt handler
    EA   = 1;                             // enable interrupts
    ET0  = 1;                             // enable timer 0 interrupt
    TMOD = (TMOD & 0xF0) | 0x01;          // set timer 0 to mode 1 (16 bit counter)
    TR0  = 1;                             // enable timer 0
}

char keypad_read() {
    char res = '\0';
    ET0 = 0;
    if( fifo_get_size( read_queue ) != 0 ) {
        res = fifo_dequeue( read_queue );
    }
    ET0 = 1;
    return res;
}
