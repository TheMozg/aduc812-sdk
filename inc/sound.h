#pragma once 

// Volume levels
#define VOL0 0
#define VOL1 0b00011100

// Note frequency data
extern char g_note_h;
extern char g_note_l;

// Set sound frequency
void set_note( char note_h, char note_l );
