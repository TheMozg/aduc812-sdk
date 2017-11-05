/*--------------------------------------------------------------
������: test_sio
��������: ���������������� ��������� RS-232. UART.
����: test_sio.c
������: ����� �., ����� �., ����� �.
��������: 	������ ��������� �������� � ���� �������.
			� ������ ������ ��������� ������������ ������� ������
			� �� (�����), � �� �� ������������� ��� ����� � ����������
			�� ����.
			�� ������ ������ ������������ �������� ������� ����
			���������� �����.
--------------------------------------------------------------*/

#include "aduc812.h"
#include "sio.h"
#include "led.h"
#define NEWLINE 0x0A
#define DEFAULT_VAL	0xFF
#define MAX_RESULT 0x63

//Global variables for mathematic construction
unsigned char mode, result, d1, d2;


/**----------------------------------------------------------------------------
                        moderes()
-------------------------------------------------------------------------------
����� ��������� ��������� ��������

����:       ���
�����:      ���
----------------------------------------------------------------------------- */
void moderes() {
		mode = 1;
		d1 = DEFAULT_VAL;
		d2 = DEFAULT_VAL;
		result = DEFAULT_VAL;
}

/**----------------------------------------------------------------------------
                        mistake()
-------------------------------------------------------------------------------
������!

����:       ���
�����:      ���
��������:	����� � �������� ��������� �� ������ � �����
			���������������� �� ���� ���� ��������� ������.
----------------------------------------------------------------------------- */
void mistake() {
			EA = 0;
			ES = 0;
			type ( "Mistake has happend\n" );
			leds( DEFAULT_VAL );
			moderes();
			EA = 1;
			ES = 1;
}

/**----------------------------------------------------------------------------
                            SIO_ISR
-------------------------------------------------------------------------------
���������� ���������� �� ����������������� ������
����:       ���
�����:      ���
���������:  � ������ ����� ����� ��� ����. ������� ����������� ��������������
			�������� �������. � ������ ���������� �������� - ���������������
			������������ ����� ���������� � ������ �������� ������.
��������:   � ������ ����� �������� �� ������ ��������� ��������� ��������
			����������� ������� � ��������. ����� ��������� �������� ����������
			��� ����� �������� '=' � '/'.
			� ������ �������� ��������������� ������������ �����, ������������
			������� (���� ����), �����, ������������ ������� � ������� ������.
-----------------------------------------------------------------------------*/
void SIO_ISR( void ) __interrupt ( 4 )
{
	unsigned char r_buf;

	//���� ���������� �� ����������� ������
	if( TI )
	{
		//0xFF - �������� �� ���������, ������� ������������
		if ( result != DEFAULT_VAL )
		{
			//0x63 - ��� ������������ ��������� ��������� �������.
			//���� ���� ����� ��� �������� \n ����� �����
			if ( result > MAX_RESULT )
			{
				SBUF = NEWLINE;
				moderes();
			}
			//������ ���� ���������� ����� - �������
			else if ( result >= 10 )
			{
				SBUF = ( result / 10 ) + 0x30;
				result = result % 10;
			}
			//������ ���� ����������� ����� - �������
			else
			{
				SBUF = result + 0x30;
				result = 0xAA;
			}
			TI = 0;
		}
	}

	//���� ���������� �� ����� ������
	if( RI )
	{
		r_buf = SBUF;
		leds( r_buf ); //��������� ���������� ������� ��� debug
		RI = 0;

		if ( r_buf >= 0x30 && r_buf <= 0x39 )
		{
			//� ����������� �� ��������� ���. �������� ���������� ���� �����
			switch ( mode )
			{
				case 1: //�������
					if ( d1 == DEFAULT_VAL )  //������ ��������� �����
						d1 = ( r_buf - 0x30 );
					else if ( d1 < 10 ) //������ ��������� �����
					{
						d1 *= 10;
						d1 += ( r_buf - 0x30 );
					}
					else 			//������� �� ���������� �����
						mistake();
					break;

				case 2: //��������
					if ( d2 == DEFAULT_VAL ) //������ ��������� �����
						d2 = ( r_buf - 0x30 );
					else  if (d2 < 10) //������ ��������� �����
					{
						d2 *= 10;
						d2 += ( r_buf - 0x30 );
					}
					else 		//������� �� ���������� �����
						mistake();
					break;

				default:
					mistake();
					break;
			}
		}

		else if ( r_buf == '/' )
		{
			mode = 2;
		}

		else if ( r_buf == '=' )
		{
			//�������� ��� ���� ������� ��� ����� � ������ ���������
			if ( d1 < 100 && d2 > 0 && d2 < 100 )
			{
				result = d1 / d2;
				TI = 1;
			}
			else
				mistake();
		}
		else //����� ������������ ������
		{
			mistake();
		}
	}
}

/**----------------------------------------------------------------------------
                            SetVectir
-------------------------------------------------------------------------------
��������� ����������������� ������� ����������.
����:
		Vector � ����� ����������� ����������,
		Address � ������ ���������������� ������� ����������.
�����:      ���
���������:  ���
��������:   �������, ��������������� ������ ���������� �
			���������������� ������ ����������
-----------------------------------------------------------------------------*/
void SetVector(unsigned char __xdata * Address, void * Vector)
{
	unsigned char __xdata * TmpVector;

	// ������ ������ �� ���������� ������ ������������ // ��� ������� �������� ���������� ljmp, ������ 02h
	*Address = 0x02;

	// ����� ������������ ����� �������� Vector
	TmpVector = (unsigned char __xdata *) (Address + 1);
	*TmpVector = (unsigned char) ((unsigned short)Vector >> 8);
	++TmpVector;
	*TmpVector = (unsigned char) Vector;

	// ����� �������, �� ������ Address ������
	// ������������� ���������� ljmp Vector
}

void main( void )
{
	unsigned char dig, i, dipval;

	SetVector( 0x2023, (void *)SIO_ISR );
	init_sio( S2400 );

	moderes();


    while( 1 )
    {
		dipval = readdip();
		//� ��������� DIP-������������� 0xFF ����� �������� ���������
		if ( dipval == DEFAULT_VAL )
		{
			//�� ������ ���������� ������ �������� ����������
			EA = 0;
			ES = 0;

			if( rsiostat() )
			{
				dig = rsio();
				if ( dig >= 0x30 && dig <= 0x39 )
				{
					for ( i = dig; i >= 0x30; i-- )
						wsio(i);
					//� ����� ������� ������� ������
					wsio(NEWLINE);
				}
			}
		}
		//� ��������� ������� DIP-������������� ����� �������� �� ����������
		else
		{
			EA=1;
			ES=1;
		}
	}
}