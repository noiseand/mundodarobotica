#define NO_BIT_DEFINES 1
#include "pic18f4550.h"
#include "pic18fregs.h"
#include <delay.h>

#define CLOCK_SPEED 48000000

void main(void) {

	TRISA = 0x2F; // 0b00101111
	TRISB = 0x03; // 0b00000011
	TRISC = 0x00; // 0b00000000
	TRISD = 0x02; // 0b00000010
	TRISE = 0x07; // 0b00000111

while(1) {
	PORTAbits.RA4 = 1;	//run
	PORTDbits.RD0 = 1;	//user
	PORTCbits.RC0 = 1;	//u2
	PORTCbits.RC1 = 1;	//u3

	delay10tcy(100000);

	PORTAbits.RA4 = 0;	//run
	PORTDbits.RD0 = 0;	//user
	PORTCbits.RC0 = 0;	//u2
	PORTCbits.RC1 = 0;	//u3

	delay10tcy(100000);

  }
}
