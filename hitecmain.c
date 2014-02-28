#include<htc.h>

void main()
{

TRISB = 0b00000011;
TRISA = 0b00101111;
TRISC = 0b00000000;
TRISD = 0b00000010;
TRISE = 0b00000111;

while (1){
	RC2 = 1;
	_delay(100000);
	RC2 = 0;
	_delay(100000);
}

}
