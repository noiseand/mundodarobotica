#include <blocos.h>

//// acelerometro

#include <MMA8452.c>

long x;
int hix,lox;

long y;
int hiy,loy;

long z;
int hiz,loz;

int8 data;

////

/////////////////////////////////////////////////////////////////
//  Function Declaration
/////////////////////////////////////////////////////////////////

void acelera();
void beep();

void ProcessInput();
void ProcessRFInput();
void init_variables();
void intro();
void Halt();
void initBoard(int beep);

/////////////////////////////////////////////////////////////////
//  Global Variables
/////////////////////////////////////////////////////////////////

//unsigned char leitura;

int gblUsbBuffer[USB_BUFFER_SIZE];
int gblUsbBufferPutIndex=0;
int gblUsbBufferGetIndex=0;
int gblUsbBufferIsFull=FALSE;


char gblFlashBuffer[getenv("FLASH_ERASE_SIZE")]; // buffer for flash write operations
char gblFlashBufferPtr=0; // pointer with-in the flash buffer
//char *gblFlashBufferPtr=0; // pointer with-in the flash buffer

int16 gblFlashBaseAddress; // where the flash buffer shuld be written to in the flash mem
//int ttTimer0 = 0;

//

void acelera() {

//printf("GOGO BOARD\r\n");
delay_ms(1000);
/*
long x;
int hix,lox;

long y;
int hiy,loy;

long z;
int hiz,loz;

int8 data;
*/
start:
data=readreg(0x0d); // who i am

printf(usb_cdc_putc,"\n\data: %0u\n", data);
if(data!=42) goto start;
MMA8452_Standby();
writeReg(CTRL_REG1,(readreg(CTRL_REG1)& ~FREAD_MASK));
MMA8452_Active();

delay_ms(100);

//

hix=readreg(OUT_X_MSB_REG);
lox=readreg(OUT_X_LSB_REG);
x = make16(hix,lox);

printf(usb_cdc_putc,"\f x: %0Lu",x);
delay_ms(100);

//

//

hiy=readreg(OUT_Y_MSB_REG);
loy=readreg(OUT_Y_LSB_REG);
y = make16(hiy,loy);

printf(usb_cdc_putc,"\f y: %0Lu",y);
delay_ms(100);

//

hiz=readreg(OUT_Z_MSB_REG);
loz=readreg(OUT_Z_LSB_REG);
z = make16(hiz,loz);

printf(usb_cdc_putc,"\f z: %0Lu\r\n",z);
delay_ms(100);

//

}

//

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
   //acelera(); // anderson
}

byte readUsbBuffer(byte *charPtr) {
   int errorCode;

   if (gblUsbBufferIsFull == TRUE) {
      gblUsbBufferIsFull = FALSE;
      errorCode = USB_OVERFLOW;
      gblUsbBufferPutIndex = 0;
      gblUsbBufferGetIndex = 0;
      *charPtr = 0;
   } else if (gblUsbBufferGetIndex == gblUsbBufferPutIndex) {
      errorCode = USB_NO_DATA;
      *charPtr = 0;
   } else {
      *charPtr = gblUsbBuffer[gblUsbBufferGetIndex];
      gblUsbBufferGetIndex++;
      if (gblUsbBufferGetIndex >= USB_BUFFER_SIZE) {
         gblUsbBufferGetIndex = 0;
      }
      errorCode = USB_SUCCESS;
   }

   return(errorCode);
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
   //int i,j;

   set_tris_a(PIC_TRIS_A);
   set_tris_b(PIC_TRIS_B);
   set_tris_c(PIC_TRIS_C);
   set_tris_d(PIC_TRIS_D);
   set_tris_e(PIC_TRIS_E);

   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(defaultPort);
   setup_adc_ports(AN0_TO_AN7_ANALOG); // Configurada as entradas analógicas RA0...RA7
   output_low(MOTOR_AB_EN);
   output_low(MOTOR_CD_EN);
   output_low(RUN_LED);
   output_low(USER_LED);
   uLED2_off();
   uLED3_off();

   setup_ccp1(CCP_PWM);
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
   setup_timer_2(T2_DIV_BY_16,250,16);
   enable_interrupts(GLOBAL);
   setup_timer_0(RTCC_INTERNAL| RTCC_DIV_256 | RTCC_8_BIT);
   set_rtcc(0);
   enable_interrupts(INT_RTCC);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(INT_TIMER2);
   enable_interrupts(INT_RDA);
   set_timer1(T1_COUNTER);
   if (beep)
      intro();
}

void flashSetWordAddress(int16 address) {

   gblFlashBaseAddress = address;
   gblFlashBaseAddress &= ~(int32)((getenv("FLASH_ERASE_SIZE"))-1);

   gblFlashBufferPtr = address - gblFlashBaseAddress;
   read_program_memory(gblFlashBaseAddress, gblFlashBuffer, getenv("FLASH_ERASE_SIZE"));
}

void flashBufferedWrite(int16 InByte) {
   gblFlashBuffer[gblFlashBufferPtr++] = (int)InByte;
   gblFlashBuffer[gblFlashBufferPtr++] = (int)(InByte>>8);

   if (!(gblFlashBufferPtr < (getenv("FLASH_ERASE_SIZE")))) {
      disable_interrupts(INT_TIMER1); // Desabilita a interrupção do timer1 para poder gravar na memória
      erase_program_eeprom(gblFlashBaseAddress);
      write_program_memory(gblFlashBaseAddress, gblFlashBuffer, getenv("FLASH_ERASE_SIZE"));
      gblFlashBufferPtr = 0;
      gblFlashBaseAddress += getenv("FLASH_ERASE_SIZE");
      read_program_memory(gblFlashBaseAddress, gblFlashBuffer, getenv("FLASH_ERASE_SIZE"));
      enable_interrupts(INT_TIMER1); // Reabilita a interrupção do timer1
   }
}


void flashFlushBuffer() {
   disable_interrupts(INT_TIMER1); // Desabilita a interrupção do timer1 para poder gravar na memória
   erase_program_eeprom(gblFlashBaseAddress);
   write_program_memory(gblFlashBaseAddress, gblFlashBuffer, getenv("FLASH_ERASE_SIZE"));
   enable_interrupts(INT_TIMER1); // Reabilita a interrupção do timer1
}

void flashWrite(int16 InByte) {
   gblFlashBuffer[gblFlashBufferPtr++] = (int)InByte;
   gblFlashBuffer[gblFlashBufferPtr++] = (int)(InByte>>8);
   flashFlushBuffer();

}

void ProcessInput() {
   //byte InByte, buff_status;
   //int1 doNotStopRunningProcedure;

   //while ((buff_status = readUsbBuffer(&InByte)) == USB_SUCCESS) {
      
      //printf(usb_cdc_putc, "%c",InByte);

   //}
}

void ReadUsb() {
   if (usb_enumerated()) { // If USB cable is connected
      while (usb_cdc_kbhit()) {
         if (gblUsbBufferIsFull == FALSE) {
            gblUsbBuffer[gblUsbBufferPutIndex] = usb_cdc_getc();
            gblUsbBufferPutIndex++;
            if (gblUsbBufferPutIndex >= USB_BUFFER_SIZE)
               gblUsbBufferPutIndex = 0;
            if (gblUsbBufferPutIndex == gblUsbBufferGetIndex)
               gblUsbBufferIsFull = TRUE;
         }
         delay_ms(5);
      }
   }
}

#include <blocos.c>

void main() {
   init_variables();
   initBoard(1);

   usb_init_cs(); // The same as usb_init(), but does not wait for the device to be connected to the bus.
                  // This is useful if your device is not bus powered and can operate without a USB connection.

   while (1) {
      usb_task(); // If you use connection sense, and the usb_init_cs() for initialization,
                  // then you must periodically call this function to keep an eye on the connection sense pin
      ReadUsb();
      ProcessInput();
      #include <programa.c>
         }

}
