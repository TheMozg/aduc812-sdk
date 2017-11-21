#pragma once

#define C_NOTE_0        16.4
#define C_SHARP_NOTE_0  17.3
#define D_NOTE_0        18.4
#define D_SHARP_NOTE_0  19.5
#define E_NOTE_0        20.6
#define F_NOTE_0        21.8
#define F_SHARP_NOTE_0  23.1
#define G_NOTE_0        24.5
#define G_SHARP_NOTE_0  26
#define A_NOTE_0        27.5
#define A_SHARP_NOTE_0  29.1
#define B_NOTE_0        30.9

#define C_NOTE_3        130.81
#define C_SHARP_NOTE_3  138.59
#define D_NOTE_3        146.83
#define D_SHARP_NOTE_3  155.56
#define E_NOTE_3        164.81
#define F_NOTE_3        174.61
#define F_SHARP_NOTE_3 	185.00
#define G_NOTE_3        196.00
#define G_SHARP_NOTE_3 	207.65
#define A_NOTE_3      	220.00
#define A_SHARP_NOTE_3	233.08
#define B_NOTE_3      	246.94

#define C_NOTE_4        261.63
#define C_SHARP_NOTE_4  277.18
#define D_NOTE_4        293.66
#define D_SHARP_NOTE_4  311.13
#define E_NOTE_4        329.63
#define F_NOTE_4        349.23
#define F_SHARP_NOTE_4 	369.99
#define G_NOTE_4        392.00
#define G_SHARP_NOTE_4 	415.30
#define A_NOTE_4      	440.00
#define A_SHARP_NOTE_4	466.16
#define B_NOTE_4      	493.88

#define C_NOTE_5        523.25
#define C_SHARP_NOTE_5  554.37
#define D_NOTE_5        587.33
#define D_SHARP_NOTE_5  622.25
#define E_NOTE_5        659.25
#define F_NOTE_5        698.46
#define F_SHARP_NOTE_5 	739.99
#define G_NOTE_5        783.99
#define G_SHARP_NOTE_5 	830.61
#define A_NOTE_5      	880
#define A_SHARP_NOTE_5	932.33
#define B_NOTE_5      	987.77

short note( int hz );
char note_h( int hz );
char note_l( int hz );

