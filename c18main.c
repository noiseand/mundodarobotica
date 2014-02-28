#include <p18F4550.h>
#include <delays.h>
#include <hardware.h>

#pragma code

void main()
{
TRISCbits.TRISC2 = 0; //Direciona somente o pino 0 da porta D como saída

while(0)
{
PORTCbits.RC2 = 1; //Coloca em '1' o bit '0' da porta D (acende o LED)
Delay10KTCYx(100); //Gera um delay de 500ms
PORTCbits.RC2 = 0; //Coloca em '0' o bit '0' da porta D (apaga o LED).
Delay10KTCYx(100); //Gera um delay de 500ms
}
}
