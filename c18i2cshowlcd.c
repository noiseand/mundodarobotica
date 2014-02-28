#include <p18F4550.h>
#include <delays.h>
#include <stdio.h>
#include <i2c.h>
#include <string.h>

#pragma code

unsigned char addr;
unsigned char data;

unsigned char databyte;

//

void semsinal(void) {

}

void recebe_i2c(void) {

if ( PIR1bits.SSPIF == 1 && SSPSTATbits.BF == 1 && SSPSTATbits.D_A == 0 )
PIR1bits.SSPIF = 0;
addr = SSPBUF;
SSPSTATbits.BF = 0;

if ( PIR1bits.SSPIF == 0 && SSPSTATbits.BF == 0 && SSPSTATbits.D_A == 1 )
data = SSPBUF;
PIR1bits.SSPIF = 0; //clear interrupt
SSPSTATbits.D_A = 0; //set D_A bit to 0 so that we can check if the subsequent byte is more data.

}

void main()
{

TRISAbits.TRISA4 = 0; // saida
TRISDbits.TRISD0 = 0; // saida
TRISCbits.TRISC0 = 0; // saida

PORTAbits.RA4 = 1; // EN LRUN

OpenI2C( SLAVE_7, SLEW_OFF);
SSPADD = 0xB0; //SSPADD contains I2C device address in SLAVE mode

//while(1)
//{

recebe_i2c();

while ( !DataRdyI2C() )
{
addr = ReadI2C(); // endereco
IdleI2C();
data = ReadI2C(); // comando
IdleI2C();
data = ReadI2C(); // parametro
IdleI2C();
data = ReadI2C(); // numero
IdleI2C();
NotAckI2C();
IdleI2C();
}

switch(data)
	{
		case 192: PORTDbits.RD0 = 1; // EN LUSR;
			break;
		default: semsinal();
			PORTDbits.RD0 = 0; // EN LUSR;
	} //case
//} // while

}
