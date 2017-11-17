#include "aduc812.h"
#include "max.h"
#include "led.h"
#include "notes.h"

#define MS_H 0xFC
#define MS_L 0x66
#define DELAY_MS 1000

static unsigned long __systime = 0;

unsigned long GetMsCounter( void ) { 
  unsigned long res; 
  ET1 = 0;
  res = __systime; 
  ET1 = 1;
  return res; 
} 

unsigned long DTimeMs( unsigned long t2 ) { 
  unsigned long t1 = GetMsCounter(); 
  return t1 - t2;   
} 

void DelayMs( unsigned long ms ) { 
  unsigned long t1 = GetMsCounter(); 
  while ( 1 ) { 
    if ( DTimeMs( t1 ) > ms ) break; 
  }   
} 

void T1_ISR ( void ) __interrupt {
    __systime++;
    
    // Cause timer passes ms every 0x043D, so 0xFFFF - 0x043D = 0xFBC3
    TH1 = MS_H;
    TL1 = MS_L;
}

char t = 0; //Флаг для переключения регистра ENA
unsigned char k = 0; //Счётчик внешних прерываний

char g_note_h;
char g_note_l;

void SetNote( char note_h, char note_l ) {
  //TH0 = note_h;
  //TL0 = note_l;
  g_note_h = note_h;
  g_note_l = note_l;
}

/* old leds - ignore for now
void T1_ISR( void ) __interrupt (1) {
	k++;
}
*/

void T0_ISR( void ) __interrupt {

		if( t ) {
      write_max(ENA, 0b0011100);
      t = 0;
    } else {
      write_max(ENA, 0b0000000);
      t = 1;
    }
    TH0 = g_note_h;
    TL0 = g_note_l;

}

void SetVector( unsigned char __xdata * Address, void * Vector ) {
	unsigned char __xdata * TmpVector;

	// Первым байтом по указанному адресу записывается // код команды передачи управления ljmp, равный 02h
	*Address = 0x02;

	// Далее записывается адрес перехода Vector
	TmpVector = (unsigned char __xdata *) (Address + 1);
	*TmpVector = (unsigned char) ((unsigned short)Vector >> 8);
	++TmpVector;
	*TmpVector = (unsigned char) Vector;

	// Таким образом, по адресу Address теперь
	// располагается инструкция ljmp Vector
}
int i;
void main( void ) {
	// Инициализация работы таймера-0

	TMOD = 0x11;
  
	//TMOD = 0x40 + 0x10 + 0x01 + 0x08; // Timer 1 counter mode + Timer 1 16 bit mode + Timer 0 16 bit mode + Timer 0 gate mode 
	//TCON = 0x40 + 0x10 + 0x01; // Timer 1 run + Timer 0 run + External interrupt INT0 edge-sensetive
	
  // Установка вектора в пользовательской таблице
	// Might work even without these if there are some goodies left in RAM
	SetVector( 0x200B, (void *)T0_ISR );
	//SetVector( 0x2013, (void *)T1_ISR );
  SetVector( 0x201B, (void *)T1_ISR );
  
  PT1 = 0;
  PT0 = 1;
  
  TH0 = A_H;
	TL0 = A_L;
  
  TH1 = MS_H;
  TL1 = MS_L;
 
  TR1 = 1;
  TR0 = 1;

  ET1 = 1;
	ET0 = 1;
  
	EX1 = 1;
  
  EA = 1;
  
  
  while( 1 )
	{
    SetNote( C_H, C_L );
    DelayMs( DELAY_MS );
    SetNote( D_H, D_L );
    DelayMs( DELAY_MS );
    SetNote( E_H, E_L );
    DelayMs( DELAY_MS );
    SetNote( F_H, F_L );
    DelayMs( DELAY_MS );
    SetNote( G_H, G_L );
    DelayMs( DELAY_MS );
    SetNote( A_H, A_L );
    DelayMs( DELAY_MS );
    SetNote( B_H, B_L );
    DelayMs( DELAY_MS );
		//leds( k );
	}
}