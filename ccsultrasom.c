#include "bootloader.h"
#case
#include <GOGO40.H>
#use i2c(MASTER,sda=PIN_B0,scl=PIN_B1,force_hw,slow)
//#use RS232 (Baud=9600, xmit = PIN_A3, rcv = PIN_A5, parity = N, ERRORS)

#include <stdlib.H>

#include <string.H>

#include <string.h>
#include <stdio.h>

#use fast_io(A)
#use fast_io(B)
#use fast_io(C)
#use fast_io(D)
#use fast_io(E)

/////////////////////////////////////////////////////////////////
//  Function Declaration
/////////////////////////////////////////////////////////////////

void beep();

void intro();
void Halt();
void initBoard(int beep);

void timer2ISR();

/////////////////////////////////////////////////////////////////
//  Global Variables
/////////////////////////////////////////////////////////////////

int16 distance, time;          // Defining variables

// Defining the pins
#define trig PIN_A3   // C0
#define echo PIN_A5   // C1

/////////////////////////////////////////////////////////////////

#int_timer2
void timer2ISR() {}

void uLED_on() {
   output_high(USER_LED);
}
void uLED_off() {
   output_low(USER_LED);
}

void uLED2_on() {
   output_high(USER_LED2);
}
void uLED2_off() {
   output_low(USER_LED2);
}

void uLED3_on() {
   output_high(USER_LED3);
}
void uLED3_off() {
   output_low(USER_LED3);
}

void beep() {
   set_pwm1_duty(50);
   delay_ms(50);
   set_pwm1_duty(0);
}

void init_variables() {
}

void intro() {
   set_pwm1_duty(50);
   USER_LED_ON;
   RUN_LED_ON;
   delay_ms(50);
   set_pwm1_duty(0);
   delay_ms(50);
   USER_LED_OFF;
   RUN_LED_OFF;
   set_pwm1_duty(50);
   delay_ms(50);
   set_pwm1_duty(0);
   delay_ms(0);
   USER_LED_ON;
   RUN_LED_ON;
   delay_ms(100);
   USER_LED_OFF;
   RUN_LED_OFF;
   output_low(PIEZO);
}

void Halt() {
   while (1) {
      output_high(RUN_LED);
      delay_ms(50);
      output_low(RUN_LED);
      delay_ms(500);
   }
}

void initBoard(int beep) {
   set_tris_a(PIC_TRIS_A);
   set_tris_b(PIC_TRIS_B);
   set_tris_c(PIC_TRIS_C);
   set_tris_d(PIC_TRIS_D);
   set_tris_e(PIC_TRIS_E);

   output_low(RUN_LED);
   output_low(USER_LED);
   uLED2_off();
   uLED3_off();

   setup_ccp1(CCP_PWM);
   setup_timer_2(T2_DIV_BY_16,250,16); // 5ms

   //
   delay_ms(1000);                              // Boot-up delay
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);     // initiating timer
   //
   if (beep)
      intro();
}

void ProcessInput() {
   byte InByte, buff_status;
   int1 doNotStopRunningProcedure;

   //while ((buff_status = readUsbBuffer(&InByte)) == USB_SUCCESS) {
}

void ReadUsb() {
   }

void main() {
   init_variables();
   initBoard(1);

   // For more information about the USB connection in a device not bus powered, see the "ex_usb_serial2.c" of PICC

   usb_init_cs(); // The same as usb_init(), but does not wait for the device to be connected to the bus.
                  // This is useful if your device is not bus powered and can operate without a USB connection.

   output_high(RUN_LED);

   while (1) {
      usb_task(); // If you use connection sense, and the usb_init_cs() for initialization,
                  // then you must periodically call this function to keep an eye on the connection sense pin

      //printf("GoGo Board\r\n");
      printf(usb_cdc_putc,"\f GoGo Board\r\n");

      output_high(trig);                         // ping the sonar
      delay_us(20);                            // sending 20us pulse
      output_low(trig);

      while(!input(echo))                       // wait for high state of echo pin
      {}

      set_timer1(0);                           // setting timer zero

      while(input(echo))                       // Wait for high state of echo pin
      {}

      time = get_timer1();                       // Getting the time

      distance = time * 0.028 + 1.093;            // Calculating the distance

      //printf("\fTime :%Lu \nDistance = %Lu",time,distance); // Putting the time on RS232 PC
      printf(usb_cdc_putc,"\fTime :%Lu \nDistance = %Lu",time,distance);

      delay_ms(1000);

   } //while
} //main
