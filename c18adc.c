#include <p18F4550.h>
#include <adc.h>
#include <delays.h>
#include <stdio.h>

unsigned long res_ad;
unsigned long tensao;
unsigned long res_temp;

#pragma code

void main()
{

OpenADC(ADC_FOSC_8 & ADC_RIGHT_JUST & ADC_4_TAD, ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, ADC_2ANA);

ADCON1 = 0x0D;

TRISAbits.TRISA0 = 1; // entrada // an0

TRISDbits.TRISD0 = 0; // saida // led

while(1)
{
SetChanADC(ADC_CH0);

ConvertADC();
while(BusyADC());
res_ad = ReadADC();

tensao = (res_ad*5000) / 1023;

if (tensao < 1000) PORTDbits.RD0 = 1;
if (tensao > 1000) PORTDbits.RD0 = 0;

Delay10KTCYx(40); //200ms

}
}
