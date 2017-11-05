/****************************************************************************

    test_sio.c - тест драйвера последовательного канала
                 для учебного стенда SDK-1.1

    (C) test_sio.c, Ключев А.О.  2007 г.

Это свободная программа; вы можете повторно распространять ее и/или
модифицировать ее в соответствии с Универсальной Общественной
Лицензией GNU, опубликованной Фондом Свободного ПО; либо версии 2,
либо (по вашему выбору) любой более поздней версии.

Эта программа распространяется в надежде, что она будет полезной,
но БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ; даже без подразумеваемых гарантий
КОММЕРЧЕСКОЙ ЦЕННОСТИ или ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ.  Для
получения подробных сведений смотрите Универсальную Общественную
Лицензию GNU.

Вы должны были получить копию Универсальной Общественной Лицензии
GNU вместе с этой программой; если нет, напишите по адресу: Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA

----------------------------------------------------------------------------
Россия, Санкт-Петербург, кафедра вычислительной техники СПбГУИТМО  
e-mail: kluchev@d1.ifmo.ru

****************************************************************************/

#include "aduc812.h"
#include "sio.h"
#include "led.h"
#include "max.h"
#define NEWLINE 0x0A
#define DEFAULT_VAL	0xFF
#define MAX_RESULT 0x63
#define MAX_KEYS_PRESSED 3
unsigned char mode, result, d1, d2;
char drebezg[4][4];
int time_count[4][4];

unsigned char temp;
unsigned char temo = 0x00;


char queue[20];
char frontQ = -1;
char endQ = -1;
char dipmod=0;

unsigned char colnum=3;
char KBTable[]="147*2580369#ABCD";

void insertQ(unsigned char a) {
	if (frontQ == 19)
		frontQ = -1;
	queue[++frontQ] = a;	
}

unsigned char getQ() {
	unsigned char symbol;
	if (endQ == 19)
		endQ = -1;
	symbol = queue[++endQ];
	return symbol;

}

unsigned char ScanKBOnce()
{
unsigned char row,col,rownum;
unsigned int i;
		
		if (colnum==3)
			colnum=0;
		else 
			colnum++;
        col = 0x1 << colnum; //0001,0010,0100,1000,0001,...

        write_max(KB, ~col); //11111110,11111101,11111011,11110111,11111110,...

        row=read_max(KB) & (0xF0);
		row=~row>>4;
    return row; 
}


void moderes() {
		mode = 1;
		d1 = 0xFF;
		d2 = 0xFF;
		result = 0xFF;

}

void mistake() {
			EA = 0;	//блокируем прерывания
			ES = 0;
			type ("Mistake has happend\n");
			leds(0xFF);
			frontQ=-1;
			endQ=-1;
			moderes();	
			EA = 1;
			ES = 1;// разрешаем прерывания от таймера для опроса		
}
void fantom_mis(){
	unsigned char i,j;
	EA=0;
	type("fantom mistake\n");
	//leds(0xAA);
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			drebezg[i][j]=0;
			time_count[i][j]=0;
		}
	EA=1;		
}

void TIMER_T2(void) __interrupt( 3 ){
	EA = 0;
	/*if (temo == 0x00)
		temo = 0xFF;
	else 
		temo = 0x00;
	leds(temo);
	*/
	TH2 = 0xFE;
	TL2 = 0x00;
	leds(0xFF);
	EA = 1;
}

void TIMER_KB(void) __interrupt( 2 ){
	unsigned char i,j,count;
	unsigned char input;
	unsigned char rownum;
	input=ScanKBOnce();//&(0x0f)
	
	for(rownum=0;rownum<4;rownum++)
	{
		count=0;
		if(input&(0x01<<rownum))
		{
			if(drebezg[colnum][rownum]==5)
				time_count[colnum][rownum]++;
			else
				drebezg[colnum][rownum]++;
		}	
		else 
		{
			if(drebezg[colnum][rownum]>0)
				drebezg[colnum][rownum]--;
			else if (drebezg[colnum][rownum]==0)
					if(time_count[colnum][rownum]>0)
						time_count[colnum][rownum]=0;
			
		}
		
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				if(drebezg[i][j] == 5)
					count = count + 1;			//for multyple pushed buttons*/
		
		
		if(count > MAX_KEYS_PRESSED)			//sSHIIIEEEET 
			fantom_mis();
			
		else
		{
			if(time_count[colnum][rownum]==1 && drebezg[colnum][rownum]==5) //first enterance
			{
				insertQ(KBTable[colnum*4+rownum]);
			}
			if(time_count[colnum][rownum]==100)
			{	//after 1 sec 300
				insertQ(KBTable[colnum*4+rownum]);
				time_count[colnum][rownum]=80; //once  250
			}
		}	
	}
	
	TH0 	  =  0xFE; // Инициализация таймера 0   d8   fe
	TL0 	  =  0xB0; //частота=10кГц(1 в 10млсек)
	//ES=1;
	//TI=1;


}

void SIO_ISR( void ) __interrupt ( 4 )
{
	if(TI)
	{
		if(dipmod==0){ //just send
			temp=getQ();
			leds(temp);
			SBUF=temp;
			TI=0;
			while(!TI);
			ES=0;
			ET0=1;
		}
		else if (dipmod==1){
		if ( result != DEFAULT_VAL )
		{
			//0x63 - это максимальный возможный результат деления.
			//Этот блок нужен для отправки \n после числа
			if ( result > MAX_RESULT )
			{
				SBUF = NEWLINE;
				moderes();
				TI=0;
				while(!TI);
				ES=0;
				ET0=1;
			}
			//Данный блок отправляет цифру - десятки
			else if ( result >= 10 )
			{
				
				SBUF = ( result / 10 ) + 0x30;
				result = result % 10;
				TI = 0;
				while(!TI);
			}
			//Данный блок отправялцет цифру - единицы
			else if (result <10)
			{
				SBUF = result + 0x30;
				result = 0xAA;
				TI = 0;
				while(!TI);
			}
		
		}
		else TI=0;
		
		}
		
	}
	
	if(RI)
	{
		leds(0xAA);
		
		
	}
}

void SetVector(unsigned char __xdata * Address, void * Vector) 
{ 
unsigned char __xdata * TmpVector; // Временная переменная
 
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
	int c;	
	unsigned char i;
	unsigned char dipval;
    
	SetVector( 0x2023, (void *)SIO_ISR );
	SetVector( 0x200B, (void *)TIMER_KB);
	SetVector( 0x202B, (void *)TIMER_T2);
	init_sio_timer( S2400 );
	
	moderes();
	
	while(1){
	if(readdip()==0x01){	//CALC	
		dipmod=1;
		if(frontQ!=endQ){
			//ES=1;				//разрешаем uart
			ET0=0; 				//запрещем прерывания таймера.
			c = getQ();			//считываем значения из буфера
			leds(c);
		
			if (c>=0x30 &&c<=0x39 ) {
				switch (mode) {
					case 1:
						if (d1 == 0xFF) 
							d1 = (c-0x30);
						else if (d1 < 10) {
							d1 *= 10;
							d1 += (c - 0x30);
						}
						else 
							mistake();
						break;
					
					case 2:
						if (d2 == 0xFF) 
							d2 = (c-0x30);
						else  if (d2 < 10){
							d2 *= 10;
							d2 += (c - 0x30);
						}
						else 
							mistake();
						break;
					default:
						mistake();
						break;
				}
			}
		
			else if (c == 'C') { //replace to D
				if(mode!=2)
					mode = 2;
				else
					mistake();
				}
			else if (c == '#') {
				if ( d1 < 100 && d2 > 0 && d2 < 100 ){
					result = d1/d2;
					leds(result);
					ES=1;
					TI=1;
				}
				else
				{
					mistake();
				}
				
			}
			
			else {
				//mistake();
				moderes();
			}
		//ET0=1; //разрешаем прерывания от таймера
		//ES=0;
		
		}
		else{
			ES=0;
			ET0=1;
		}
	}	
		
	else {				//just read from keyboard
		dipmod=0;
		if(frontQ!=endQ){
		ES=1;
		ET0=0; 
		TI=1;
		}
		else{
			ES=0;
			ET0=1;
		}
	}
	

}
}    
