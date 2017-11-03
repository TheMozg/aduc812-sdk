#include "aduc812.h"
#include "max.h"
#include "led.h"

void delay ( unsigned long ms ) {
	volatile unsigned long i, j;

    for( j = 0; j < ms; j++ ) {
        for( i = 0; i < 50; i++ );
    }
}

int i = 0; //Счётчик для регулирования времени исполнения ноты
char t = 0; //Флаг для переключения регистра ENA
unsigned char k = 0; //Счётчик внешних прерываний

void T1_ISR( void ) __interrupt 1 {
	k++;
}

/**----------------------------------------------------------------------------
                            T0_ISR
-------------------------------------------------------------------------------
Обработчик прерывания по таймеру 0
Вход:       нет
Выход:      нет
Результат:  нет
Описание:   Устанавливается определённое значение в регистр прерываний
			для конфигурации частоты прерываний (нужно для выработки
			звука нужной частоты).
-----------------------------------------------------------------------------*/
void T0_ISR( void ) __interrupt 2 {
	if( i <= 500 && t == 0 )
	{
		//Нота Си - нижняя часть
		write_max(ENA, 0b0100000);
		t = 1;
		i++;
		TH0 = 0xFC;
		TL0 = 0x17;
	}
	else if ( i <= 500 && t == 1 )
	{
		//Нота Си - верхняя часть
		write_max(ENA, 0b0111100);
		t=0;
		i++;
		TH0 = 0xFC;
		TL0 = 0x17;
	}
	else if( i > 500 && i <= 948 && t==0 )
	{
		//Нота Ля - нижняя часть
		write_max(ENA, 0b0100000);
		t = 1;
		i++;
		TH0 = 0xFB;
		TL0 = 0x8B;
	}
	else if( i > 500 && i <= 948 && t == 1 )
	{
		//Нота Ля - верхняя часть
		write_max(ENA, 0b0111100);
		t = 0;
		i++;
		TH0 = 0xFB;
		TL0 = 0x8B;
	}
	else if( i > 948 && i <= 1338 && t == 0 )
	{
		//Нота Соль - нижняя часть
		write_max(ENA, 0b0100000);
		t = 1;
		i++;
		TH0 = 0xFA;
		TL0 = 0xFF;
	}
	else if( i > 948 && i <= 1338 && t== 1 )
	{
		//Нота Соль - верхняя часть
		write_max(ENA, 0b0111100);
		t = 0;
		i++;
		TH0 = 0xFA;
		TL0 = 0xFF;
	}
	else if( i > 1338 && i <= 1687 && t==0 )
	{
		//Нота Фа - нижняя часть
		write_max(ENA, 0b0100000);
		t = 1;
		i++;
		TH0 = 0xFA;
		TL0 = 0x69;
	}
	else if( i > 1338 && i <= 1687 && t==1 )
	{
		//Нота Фа - верхняя часть
		write_max(ENA, 0b0111100);
		t = 0;
		i++;
		TH0 = 0xFA;
		TL0 = 0x69;
	}
	else if( i > 1687 && i <= 2016 && t==0 )
	{
		//Нота Ми - нижняя часть
		write_max(ENA, 0b0100000);
		t = 1;
		i++;
		TH0 = 0xFA;
		TL0 = 0x0F;
	}
	else if( i > 1687 && i <= 2016 && t==1 )
	{
		//Нота Ми - верхняя часть
		write_max(ENA, 0b0111100);
		t = 0;
		i++;
		TH0 = 0xFA;
		TL0 = 0x0F;
	}
	else if( i > 2016 && i <= 2310 && t==0 )
	{
		//Нота Ре - нижняя часть
		write_max(ENA, 0b0100000);
		t = 1;
		i++;
		TH0 = 0xF9;
		TL0 = 0x5B;
	}
	else if( i > 2016 && i <= 2310 && t==1 )
	{
		//Нота Ре - верхняя часть
		write_max(ENA, 0b0111100);
		t = 0;
		i++;
		TH0 = 0xF9;
		TL0 = 0x5B;
	}
	else if( i > 2310 && i <= 2571 && t==0 )
	{
		//Нота До - нижняя часть
		write_max(ENA, 0b0100000);
		t = 1;
		i++;
		TH0 = 0xF8;
		TL0 = 0x89;
	}
	else if( i > 2310 && i <= 2571 && t==1 )
	{
		//Нота До - верхняя часть
		write_max(ENA, 0b0111100);
		t = 0;
		i++;
		TH0 = 0xF8;
		TL0 = 0x89;
	}
	else
	{
		//Начать проигрывать мелодию заново
		i = 0;
		t = 0;
	}
}

void SetVector(unsigned char __xdata * Address, void * Vector)
{
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

void main( void )
{
	// Инициализация работы таймера-0
	TH0 = 0x00;
	TL0 = 0x00;
	TMOD = 0x01;
	TCON = 0x14;

	// Установка вектора в пользовательской таблице
	// Might work even without these if there are some goodies left in RAM
	SetVector( 0x20B, (void *)T0_ISR );
	SetVector( 0x2013, (void *)T1_ISR );

	// Разрешение прерываний от таймеров
	EA = 1;
	ET0 = 1;
	EX1 = 1;

	while( 1 )
	{
		leds( k );
	}
}