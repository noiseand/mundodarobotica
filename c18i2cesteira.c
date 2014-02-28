#include <p18F4550.h>
#include <delays.h>
#include <i2c.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma code

// definicoes

int leds;
int motor;
int sensor;
int potencia;
int ciclopwm;
int modoburst;

int addr;
int data;

// funcoes

#include "led.c"

#include "init.c"

#include "motora.c"
#include "motorb.c"
#include "motorc.c"
#include "motord.c"

//#include "beep.c"

void semsinal(void) {
	//init_board();
	}

//

//SSPBUF : Serial TX/RX data buffer.
//PIR1bits.SSPIF : Interrupt flag bit. This will be 1 when data is received into SSPBUF
//SSPSTATbits.BF : SSPBUF buffer full status bit. 1 = full, 0 = empty.
//SSPSTATbits.D_A : Data / Address bit. When receiving it indicates what the data received was. 0 = address, 1 = data.

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

#include "pinos.c"

init_board();
//beep();

OpenI2C( SLAVE_7, SLEW_OFF);
SSPADD = 0xB0; //SSPADD contains I2C device address in SLAVE mode

while(1)
{

recebe_i2c();

switch(data)
	{
		case 64: liga_mA();
			liga_mB();
			liga_mC();
			liga_mD();
			break;
		case 68: desliga_mA();
			desliga_mB();
			desliga_mC();
			desliga_mD();
			break;
		case 82: reverte_mA();
			reverte_mB();
			reverte_mC();
			reverte_mD();
			break;
		case 84: parar_mA();
			//parar_mB();
			//parar_mC();
			//parar_mD();
			break;
		case 192: led_on();
			break;
		case 193: led_off();
			break;
		default: semsinal();
			desliga_mA();
	} //case
} // while
} // main
