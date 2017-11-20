#ifndef __SIO__H
#define __SIO__H

//Константы скорости
#define S9600   0xFD
#define S4800   0xFA
#define S2400   0xF4
#define S1200   0xE8

enum mode {
  SYNC,
  ASYNC
};

extern enum mode g_mode;

unsigned char rsiostat(void);
void wsio( unsigned char c );
unsigned char rsio(void);
void type(char * str);
void init_sio( unsigned char speed  );

#endif // __SIO__H
