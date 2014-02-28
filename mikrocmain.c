#pragma orgall 0x1560

void main() org 0x1560 {

TRISB = 0b00000011;
TRISA = 0b00101111;
TRISC = 0b00000000;
TRISD = 0b00000010;
TRISE = 0b00000111;

do {

RD0_bit = 0;
RC0_bit = 0;
RC1_bit = 0;
Delay_ms(1000); // 1 second delay

RD0_bit = 1;
RC0_bit = 1;
RC1_bit = 1;
Delay_ms(1000); // 1 second delay
} while(1); // Endless loop
}
