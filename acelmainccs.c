//
// Copyright (C) 2001-2007 Massachusetts Institute of Technology
// Contact   Arnan (Roger) Sipiatkiat [arnans@gmail.com]


//************************* GOGO BR  *******************************************//
//*****  contact  Felipe Augusto Silva  *****************************************//
//*****  email:  fel1310@hotmail.com   *****************************************//
//******************************************************************************//

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#include "bootloader.h"
#case
#include <GOGO40.H>
#use i2c(MASTER,sda=PIN_B0,scl=PIN_B1,force_hw,slow)

#include <logovm.h>
#include <stdlib.H>

#use fast_io(A)
#use fast_io(B)
#use fast_io(C)
#use fast_io(D)
#use fast_io(E)

#define defaultPort      0
#define channelSwitchDelay   50   // delay time in us after switching adc channels
// Don't decrease this value without testing.
// If the delay is too short (i.e. 10us) the adc won't
// have enough time to stabilize before reading the
// next channel.
//#define T1_COUNTER 50536 // Interrupção a cada 10ms
//#define T1_COUNTER 64036 // Interrupcao a cada 1ms
#define T1_COUNTER 28035 // Interrupacao a cada 25mseg

//#define  CMD_TIMEOUT_PERIOD  2     // determins how long befor the board will reset
#define  CMD_TIMEOUT_PERIOD  200     // determins how long befor the board will reset
// the command state. Units in 1/10 of a second

// Constants used in SETUP_ADC_PORTS() are:
#define ALL_ANALOG   0x00   // A0 A1 A2 A3 A5 E0 E1 E2 B2 B3 B1 B4 B0
#define AN0_TO_AN7   0x07   // A0 A1 A2 A3 A5 E0 E1 E2

//// acelerometro

#include <MMA8452.c>

////

/////////////////////////////////////////////////////////////////
//  Function Declaration
/////////////////////////////////////////////////////////////////
void setHigh(IOPointer Pin);
void setLow(IOPointer Pin);
int  readPin(IOPointer Pin);

short getBit(int InByte, int BitNo);
void setBit(int *InByte, int BitNo);
void clearBit(int *InByte, int BitNo);

void Ping(int Param);
void TalkToMotor(int MotorBits);
void MotorControl(int MotorCmd);
void SetMotorPower(int Power);
void ChangeMotorPower(int delta);
void sortMtrDuty();
void SetMotorMode(int motorMode); // normal or servo

void ENHigh(int groupNo);
void ENLow(int groupNo);

void MotorON(int MotorNo);
void MotorOFF(int MotorNo);
void MotorRD(int MotorNo);
void MotorThisWay(int MotorNo);
void MotorThatWay(int MotorNo);
void MotorCoast(int MotorNo);
void miscControl(int cur_param, int cur_ext, int cur_ext_byte);

void acelera();
void beep();

void SetBurstMode(int SensorBits, int Mode);
void DoSensorStuff();
unsigned int16 readSensor(int sensorNo);
long getSensorVal();
void switchAdcChannel(int channelNo);

void ProcessInput();
void ProcessRFInput();
void init_variables();
void intro();
void Halt();
void initBoard(int beep);

void timer2ISR();
void version();

/////////////////////////////////////////////////////////////////
//  Global Variables
/////////////////////////////////////////////////////////////////

IOPointer  MotorENPins [MotorCount]={  MTR1_EN, MTR2_EN, MTR3_EN, MTR4_EN};
IOPointer  MotorCWPins [MotorCount]={  MTR1_CW, MTR2_CW, MTR3_CW, MTR4_CW};
IOPointer  MotorCCPins [MotorCount]={  MTR1_CC, MTR2_CC, MTR3_CC, MTR4_CC};

unsigned int CMD_STATE;

unsigned char leitura;

int gbl_cur_cmd, gbl_cur_param, gbl_cur_ext, gbl_cur_ext_byte;
int gblBurstModeBits;
int gblBurstModeCounter=0;   // tracks which sensor is the current burst mode sensor

int1 gblSlowBurstMode=0;  // determinds which burst mode we're in (0=normal, 1=slow)
int1 gblSlowBurstModeTimerHasTicked=0;  // ticks every 1/72 sec (by timer0)


int gblCurSensorChannel;

int gblMotorMode=0b00000000;   // default to normal mode
int gblActiveMotors = 0;
int gblMotorDir=0;
int gblMotorONOFF = 0;
int gblMtrDuty[MotorCount+1] = {0xff,0xff,0xff,0xff,0xff};  // Motor PWM Duty cycle
int gblMtrCurrDuty[MotorCount+1] = {0x00,0x00,0x00,0x00,0x00};  // Motor PWM Duty cycle
unsigned int gblTimer0Counter = MotorCount; // Motor duty cycle counter.
unsigned int gblDutyCycleFlag = 0; // used to find the next duty cycle in timer0
unsigned int gblCurrentDutyIndex = 0; // keeps track of the current duty cycle being used.

// These two variables are for the NEWIR, IR commands in Cricket Logo
// We replace the IR with the serial comm, of course.
unsigned char gblMostRecentlyReceivedByte;
int1 gblNewByteHasArrivedFlag = 0;


int1 gblLogoIsRunning = 0;     // flags if logo procedures are runing
int1 gblButtonPressed = 0;    // flags when the run button is pressed
int1 gblBtn1AlreadyPressed = 0;
int1 gblTurnedOff = 0;unsigned int16 gblWaitCounter = 0;  // used for the wait cmd in Logo vm
int1 gblCmdRUN = 0;
unsigned int32 gblTimer = 0;   // This is the timer for the TIMER and RESETT commands

int gblCmdTimeOut = 0; // counter to make sure the command state is not stuck somewhere

int gblUsbBuffer[USB_BUFFER_SIZE];
int gblUsbBufferPutIndex=0;
int gblUsbBufferGetIndex=0;
int gblUsbBufferIsFull=FALSE;


char gblFlashBuffer[getenv("FLASH_ERASE_SIZE")]; // buffer for flash write operations
char gblFlashBufferPtr=0; // pointer with-in the flash buffer
//char *gblFlashBufferPtr=0; // pointer with-in the flash buffer

int16 gblFlashBaseAddress; // where the flash buffer shuld be written to in the flash mem
int ttTimer0 = 0;

void version()
{
   printf(usb_cdc_putc,"4");
}

void prtmsg1 ()
{
printf(usb_cdc_putc,"RECORD\n");
}
void prtmsgRESETDP()
{
printf(usb_cdc_putc,"RESETDP\n");
}
void prtmsg2()
{
printf(usb_cdc_putc,"Recall\n");
}
void prtfbprt() //, int16 op2, byte op3)
{
      printf(usb_cdc_putc_fast,"\n\r gblFlashBufferPtr=%d ", gblFlashBufferPtr,);
      // printf(usb_cdc_putc,"op1:%ld op2:%ld %c", op1,op2,op3);
}

void prta(signed int16 op1) //, int16 op2, byte op3)
{
      printf(usb_cdc_putc_fast,"%ld%c", op1,' ');
      // printf(usb_cdc_putc,"op1:%ld op2:%ld %c", op1,op2,op3);
}
void prtFLBBF()
{
int8 i;
printf(usb_cdc_putc,"\n\rFlbf");
   for(i=0;i<getenv("FLASH_ERASE_SIZE");i++)
   printf(usb_cdc_putc," %d-%2d ",i,gblFlashBuffer[i]);
}




#int_rtcc
void clock_isr() {
   int i;
   unsigned int minDuty;
   unsigned int nextDutyIndex;
   unsigned int periodTilNextInterrupt;
   ttTimer0++;
   if (ttTimer0 == 2) {
      ttTimer0 = 0;
      do {
         if (gblTimer0Counter < MotorCount) {
            if (getBit(gblMotorONOFF, gblCurrentDutyIndex) == ON) {
               if ((gblMtrDuty[gblCurrentDutyIndex] + gblMtrCurrDuty[gblCurrentDutyIndex])/2 < 255) {
                  if (getBit(gblMotorMode, gblCurrentDutyIndex) == MOTOR_NORMAL) {
                     if (getBit(gblMotorDir, gblCurrentDutyIndex))
                        output_low(MotorCWPins[gblCurrentDutyIndex]);
                     else
                        output_low(MotorCCPins[gblCurrentDutyIndex]);
                  }
                  else
                     output_low(MotorCCPins[gblCurrentDutyIndex]);
              }
            }
         }
         else {
            for (i=0 ; i<MotorCount ; i++) {
               if (getBit(gblMotorONOFF, i) == ON) {
                  if (gblMtrDuty[i] > 0) {
                     if (getBit(gblMotorMode, i) == MOTOR_NORMAL) {
                        if (getBit(gblMotorDir, i))
                           output_high(MotorCWPins[i]);
                        else
                           output_high(MotorCCPins[i]);
                     }
                     else
                        output_high(MotorCCPins[i]);
                  }
               }
            }
         }
         minDuty = 255;
         for (i=0;i<=MotorCount;i++) {
            if ((minDuty >= gblMtrDuty[i]) && !(getBit(gblDutyCycleFlag,i))) {
               minDuty = gblMtrDuty[i];
               nextDutyIndex = i;
            }
         }
         setBit(&gblDutyCycleFlag, nextDutyIndex);
         if (gblTimer0Counter < MotorCount)
            periodTilNextInterrupt = minDuty - gblMtrDuty[gblCurrentDutyIndex];
         else
            periodTilNextInterrupt = minDuty;
         gblCurrentDutyIndex = nextDutyIndex;
         if (gblTimer0Counter == MotorCount-1)
            gblDutyCycleFlag = 0;
         if (gblTimer0Counter < MotorCount)
            gblTimer0Counter++;
         else
            gblTimer0Counter = 0;
      } while ((periodTilNextInterrupt == 0) && (gblTimer0Counter > 0));


      if (gblTimer0Counter == 0) {
         gblSlowBurstModeTimerHasTicked=1;
      }
   }
   set_rtcc(255-periodTilNextInterrupt);
}

#int_timer1
void timer1ISR() {
   gblTimer+= 25; // since the current timer1 Interruption is every 25mseg

   if (CMD_STATE != WAITING_FOR_FIRST_HEADER) {
      gblCmdTimeOut+= 25; // this count  is  absolute in mseg
   }

   if (gblWaitCounter > 0 && gblTimer%100 == 0) {
      gblWaitCounter--;
   }

   // Para gerar a rampa de aceleração
   if (gblMtrCurrDuty[gblCurrentDutyIndex] < gblMtrDuty[gblCurrentDutyIndex]) {
       // 0 a 255 em 300 ms
      gblMtrCurrDuty[gblCurrentDutyIndex] += 21;
      gblMtrCurrDuty[gblCurrentDutyIndex] = (gblMtrCurrDuty[gblCurrentDutyIndex] > 255 ? 255 : gblMtrCurrDuty[gblCurrentDutyIndex]);
   }

   if (input(RUN_BUTTON) || gblCmdRUN) {
      if (!gblBtn1AlreadyPressed || gblCmdRUN) {
         gblCmdRUN = 0;
         gblButtonPressed = !gblButtonPressed;
         gblBtn1AlreadyPressed = 1;
         gblWaitCounter = 0;
         gblONFORNeedsToFinish = 0;
         if (!gblLogoIsRunning) {
            srand(gblTimer);
            output_high(RUN_LED);
            //leitura da regiao onde comeÃ§ara o codigo logo
            gblMemPtr = (read_program_eeprom(RUN_BUTTON_BASE_ADDRESS)<<8)+read_program_eeprom(RUN_BUTTON_BASE_ADDRESS+2);
            gblMemPtr *= 2;
            clearStack();
            gblNewByteHasArrivedFlag=0;
            gblLogoIsRunning = 1;
         } else {
            gblLogoIsRunning = 0;
            gblTurnedOff = 1;
            output_low(RUN_LED);
         }
      }
   } else if (gblBtn1AlreadyPressed) {
      gblBtn1AlreadyPressed=0;
   }

   set_timer1(T1_COUNTER);
}

#int_timer2
void timer2ISR() {}

short getBit(int InByte, int BitNo) {
   return ((InByte >> BitNo) & 1);
}

void setBit(int *InByte, int BitNo) {
   *InByte |= (1<<BitNo);
}

void clearBit(int *InByte, int BitNo) {
   *InByte &= ~(1<<BitNo);
}

void TalkToMotor(int MotorBits) {
   gblActiveMotors = MotorBits;
}

void MotorControl(int MotorCmd) {
   int i;
   for (i=0;i<MotorCount;i++) {
      if ((gblActiveMotors >> i) & 1) {
         SetMotorMode(MOTOR_NORMAL);
         switch (MotorCmd) {
            case MTR_ON:
               MotorON(i);
               break;
            case MTR_OFF:
               MotorOFF(i);
               break;
            case MTR_RD:
               MotorRD(i);
               break;
            case MTR_THISWAY:
               MotorThisWay(i);
               break;
            case MTR_THATWAY:
               MotorThatWay(i);
               break;
            case MTR_COAST:
               MotorCoast(i);
               break;
         }
      }
   }
}

void SetMotorPower(int Power) {
   int i;
   for (i=0;i<MotorCount;i++) {
      if ((gblActiveMotors >> i) & 1) {
         switch (Power) {
            case 1:
               Power = 36;
               break;
            case 2:
               Power = 73;
               break;
            case 3:
               Power = 110;
               break;
            case 4:
               Power = 146;
               break;
            case 5:
               Power = 183;
               break;
            case 6:
               Power = 219;
               break;
            case 7:
               Power = 255;
               break;
         }
         gblMtrCurrDuty[i] = 0;
         gblMtrDuty[i] = Power;
      }
   }
}

void ChangeMotorPower(int delta) {
   int i;
   for (i=0;i<MotorCount;i++) {
      if ((gblActiveMotors >> i) & 1) {
         gblMtrDuty[i] = gblMtrDuty[i] + delta;
      }
   }
}

void SetMotorMode(int motorMode) {
   int i;
   for (i=0;i<MotorCount;i++) {
      if ((gblActiveMotors >> i) & 1)
         if (motorMode == MOTOR_NORMAL)
            clearBit(&gblMotorMode, i);
         else
            setBit(&gblMotorMode, i);
   }
}

void ENHigh(int MotorNo) {
 //   int foo;
  //  foo = MotorNo - MotorNo%2;
   output_high(MotorENPins[MotorNo]);
   //if (foo == 0) {
      output_high(MOTOR_AB_EN);
   //} else {
      output_high(MOTOR_CD_EN);
   //}
}

void ENLow(int MotorNo) {
    int foo;
   output_low(MotorENPins[MotorNo]);
    if(MotorNo%2)
    {
        foo=MotorNo-1;
    }
    else
    {
        foo=MotorNo+1;
    }
    if (!(getBit(gblMotorONOFF,foo)))
    {
        return;
    }

   if (MotorNo<2)
    {
      output_low(MOTOR_AB_EN);
        return;
   }
    else
    {
      output_low(MOTOR_CD_EN);
        return;
   }
}


void MotorON(int MotorNo) {
   IOPointer MtrCC, MtrCW;
   MtrCW = MotorCWPins[MotorNo];
   MtrCC = MotorCCPins[MotorNo];
   if (getBit(gblMotorDir,MotorNo)) {
      output_low(MtrCC);
      output_high(MtrCW);
   } else {
      output_high(MtrCC);
      output_low(MtrCW);
   }
   ENHigh(MotorNo);
   setBit(&gblMotorONOFF,MotorNo);
}

void MotorOFF(int MotorNo) {
   IOPointer MtrCC, MtrCW;
   MtrCW = MotorCWPins[MotorNo];
   MtrCC = MotorCCPins[MotorNo];
   output_high(MtrCC);
   output_high(MtrCW);
    output_low(MotorENPins[MotorNo]);
    ENLow(MotorNo);
   clearBit(&gblMotorONOFF,MotorNo);
}

void MotorRD(int MotorNo) {
   IOPointer MtrCC, MtrCW;
   MtrCW = MotorCWPins[MotorNo];
   MtrCC = MotorCCPins[MotorNo];
   if (getBit(gblMotorDir,MotorNo))
    {
        output_low(MtrCW);
      output_high(MtrCC);
        clearBit(&gblMotorDir,MotorNo);
   }
    else
    {
        output_high(MtrCW);
      output_low(MtrCC);
      setBit(&gblMotorDir,MotorNo);
   }
}

void MotorThisWay(int MotorNo) {
   IOPointer MtrCC, MtrCW;
   MtrCW = MotorCWPins[MotorNo];
   MtrCC = MotorCCPins[MotorNo];
   setBit(&gblMotorDir,MotorNo);
   output_low(MtrCC);
   output_high(MtrCW);
}


void MotorThatWay(int MotorNo) {
   IOPointer MtrCC, MtrCW;
   MtrCW = MotorCWPins[MotorNo];
   MtrCC = MotorCCPins[MotorNo];
   clearBit(&gblMotorDir,MotorNo);
   output_low(MtrCW);
   output_high(MtrCC);
}

void MotorCoast(int MotorNo) {
   IOPointer MtrCC, MtrCW;
   MtrCW = MotorCWPins[MotorNo];
   MtrCC = MotorCCPins[MotorNo];
   clearBit(&gblMotorONOFF,MotorNo);
   output_low(MtrCW);
   output_low(MtrCC);
}

void miscControl(int cur_param, int cur_ext, int cur_ext_byte) {
   switch (cur_param) {
      case MISC_USER_LED:
         if (cur_ext == TURN_USER_LED_ON) {
            USER_LED_ON;
         } else {
            USER_LED_OFF;
         }
         break;
      case MISC_BEEP:
         beep();
         break;
      case MISC_SET_PWM:
         MotorControl(MTR_ON);
         MotorControl(MTR_THISWAY);
         SetMotorMode(MOTOR_SERVO);
         SetMotorPower(cur_ext_byte);
         break;
      case MISC_UPLOAD_EEPROM:
         break;
      case MISC_I2C_SETUP:
         switch (cur_ext) {
            case I2C_START:
               i2c_start();
               break;
            case I2C_STOP:
               i2c_stop();
               break;
            case I2C_WRITE:
               i2c_write(cur_ext_byte);
               break;
            case I2C_READ:
               i2c_read(0);
               break;
         }
         break;
   }
}

//

void acelera() {

long x;
int hix,lox;

long y;
int hiy,loy;

long z;
int hiz,loz;

int8 data;

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
   //set_pwm1_duty(50);
   //delay_ms(50);
   //set_pwm1_duty(0);
   acelera(); // anderson
}

void DoSensorStuff() {
   long SensorVal;

   if (!gblSlowBurstMode || gblSlowBurstModeTimerHasTicked) {
      if ((gblBurstModeBits>>gblBurstModeCounter) & 1) {
         SensorVal=readSensor(gblBurstModeCounter);
         printf(usb_cdc_putc,"%c%c%c", 0x0c, (gblBurstModeCounter << 5) | (SensorVal >> 8), SensorVal & 0xff);
      }
      gblBurstModeCounter = (gblBurstModeCounter+1) % 8;
      gblSlowBurstModeTimerHasTicked = 0;
   }
}

void SetBurstMode(int SensorBits, int Mode) {
   gblBurstModeBits = SensorBits;
   if (Mode > 0)
      gblSlowBurstMode = 1;
   else
      gblSlowBurstMode = 0;
}

unsigned int16 readSensor(int sensorNo) {

   if (gblCurSensorChannel != sensorNo) {
      switchAdcChannel(sensorNo);
      gblCurSensorChannel=sensorNo;
   }
   return(getSensorVal());
}


long getSensorVal() {
   long sensorVal;
   sensorVal=read_adc();
   return (sensorVal);
}


void switchAdcChannel(int channelNo) {
   set_adc_channel(channelNo);
   delay_us(channelSwitchDelay);
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
   gblBurstModeBits = 0;
   CMD_STATE = WAITING_FOR_FIRST_HEADER;
   gblLogoIsRunning=0;
   gblStkPtr=0;
   gblInputStkPtr=0;
   gblErrFlag=0;
   gblRecordPtr = read_program_eeprom(MEM_PTR_LOG_BASE_ADDRESS);
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
   int i,j;

   gblActiveMotors = 0;
   set_tris_a(PIC_TRIS_A);
   set_tris_b(PIC_TRIS_B);
   set_tris_c(PIC_TRIS_C);
   set_tris_d(PIC_TRIS_D);
   set_tris_e(PIC_TRIS_E);

   setup_adc(ADC_CLOCK_INTERNAL);
   gblCurSensorChannel=defaultPort;
   set_adc_channel(defaultPort);
   setup_adc_ports(AN0_TO_AN7_ANALOG); // Configurada as entradas analógicas RA0...RA7
   output_low(MOTOR_AB_EN);
   output_low(MOTOR_CD_EN);
   output_low(RUN_LED);
   output_low(USER_LED);
   uLED2_off();
   uLED3_off();
   for (i=0,j=0 ; i<MotorCount ; i++) {
      output_low(MotorENPins[i]);
      output_low(MotorCWPins[i]);
      output_low(MotorCCPins[i]);
   }

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
   byte InByte, buff_status;
   int1 doNotStopRunningProcedure;

   while ((buff_status = readUsbBuffer(&InByte)) == USB_SUCCESS) {
      gblCmdTimeOut = 0 ;
      gblMostRecentlyReceivedByte = InByte;
      gblNewByteHasArrivedFlag = 1;


      printf(usb_cdc_putc, "%c",InByte);

      switch (CMD_STATE) {
         case WAITING_FOR_FIRST_HEADER:
            switch (InByte) {
               case InHeader1:
                  CMD_STATE = WAITING_FOR_SECOND_HEADER;
                  doNotStopRunningProcedure = 1;
                  break;
               case SET_PTR:
                 init_variables();
                 initBoard(0);
                 CMD_STATE = SET_PTR_HI_BYTE;
                  break;
               case READ_BYTES:
                  CMD_STATE = READ_BYTES_COUNT_HI;
                  break;
               case WRITE_BYTES:
                  CMD_STATE = WRITE_BYTES_COUNT_HI;
                  break;
               case RUN:
                  gblCmdRUN = 1;
                  doNotStopRunningProcedure = 1;
                  CMD_STATE = WAITING_FOR_FIRST_HEADER;
                  break;
               case CRICKET_CHECK:
                  CMD_STATE = CRICKET_NAME;
                  break;
            };
            if (!doNotStopRunningProcedure) {
               gblLogoIsRunning = 0;
               gblWaitCounter = 0;
               output_low(RUN_LED);
               gblBurstModeBits = 0;
            }
            doNotStopRunningProcedure=0;
            break;
         case WAITING_FOR_SECOND_HEADER:
            if (InByte == InHeader2) CMD_STATE = WAITING_FOR_CMD_BYTE;
            break;
         case WAITING_FOR_CMD_BYTE:
            gbl_cur_cmd =(InByte & 0b11100000) >> 5;
            gbl_cur_param =(InByte & 0b00011100) >> 2;
            gbl_cur_ext =(InByte & 0b00000011);
            if (gbl_cur_cmd > ONE_BYTE_CMD) {
               CMD_STATE = WAITING_FOR_SECOND_CMD_BYTE;
            } else {
               CMD_STATE = CMD_READY;
            }
            break;
         case WAITING_FOR_SECOND_CMD_BYTE:
            gbl_cur_ext_byte = InByte;
            CMD_STATE = CMD_READY;
            break;
         case CMD_READY:
            break;
         case SET_PTR_HI_BYTE:
            gblMemPtr = (unsigned int16) InByte << 8;
            CMD_STATE = SET_PTR_LOW_BYTE;
            break;
         case SET_PTR_LOW_BYTE:
            gblMemPtr = gblMemPtr | InByte;
            CMD_STATE = WAITING_FOR_FIRST_HEADER;
            if ((gblMemPtr&0xff0) == 0xff0) {
               gblMemPtr = (RUN_BUTTON_BASE_ADDRESS + ((gblMemPtr&0xf)*2)) - FLASH_USER_PROGRAM_BASE_ADDRESS ;
               set_pwm1_duty(50);
            } else {
               gblMemPtr *= 2;
            }
            flashSetWordAddress(FLASH_USER_PROGRAM_BASE_ADDRESS + gblMemPtr);
            break;
         case READ_BYTES_COUNT_HI:
            gblRWCount = (unsigned int16) InByte << 8;
            CMD_STATE = READ_BYTES_COUNT_LOW;
            break;
         case READ_BYTES_COUNT_LOW:
            gblRWCount = gblRWCount | InByte;
            sendBytes(gblMemPtr, gblRWCount);
            gblMemPtr += gblRWCount;
            CMD_STATE = WAITING_FOR_FIRST_HEADER;
            break;
         case WRITE_BYTES_COUNT_HI:
            gblRWCount = (unsigned int16) InByte << 8;
            CMD_STATE = WRITE_BYTES_COUNT_LOW;
            break;
         case WRITE_BYTES_COUNT_LOW:
            gblRWCount = gblRWCount | InByte;
            CMD_STATE = WRITE_BYTES_SENDING;
            break;
         case WRITE_BYTES_SENDING:
            set_pwm1_duty(0);
            flashBufferedWrite(InByte);
            if (--gblRWCount < 1) {
               flashFlushBuffer();
               CMD_STATE = WAITING_FOR_FIRST_HEADER;
            }
            printf(usb_cdc_putc,"%c",255-InByte);
            break;
         case CRICKET_NAME:
            printf(usb_cdc_putc,"%c",0x37);
            CMD_STATE = WAITING_FOR_FIRST_HEADER;
            break;
         default:
            CMD_STATE = WAITING_FOR_FIRST_HEADER;
            break;
      }
      if (CMD_STATE == CMD_READY) break;
   }

   if (buff_status == USB_OVERFLOW) {
      CMD_STATE = WAITING_FOR_FIRST_HEADER;
   }
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

void main() {
   int16 SensorVal;
   int16 uploadLen,counter;
   int16 foo;
   int16 ping = 0; // Para garantir compatibilidade com o monitor de Stanford

   init_variables();
   initBoard(1);

   // For more information about the USB connection in a device not bus powered, see the "ex_usb_serial2.c" of PICC

   usb_init_cs(); // The same as usb_init(), but does not wait for the device to be connected to the bus.
                  // This is useful if your device is not bus powered and can operate without a USB connection.

   while (1) {
      usb_task(); // If you use connection sense, and the usb_init_cs() for initialization,
                  // then you must periodically call this function to keep an eye on the connection sense pin
      ReadUsb();
      ProcessInput();
      if (CMD_STATE == CMD_READY) {
         switch (gbl_cur_cmd) {
            case CMD_PING:
               ping = 1; // Para garantir compatibilidade com o monitor de Stanford
               break;
            case CMD_Version:
               version();
               break;
            case CMD_READ_SENSOR:
               SensorVal = readSensor(gbl_cur_param);
               break;
            case CMD_MOTOR_CONTROL:
               MotorControl(gbl_cur_param);
               break;
            case CMD_MOTOR_POWER:
               SetMotorPower(gbl_cur_param);
               break;
            case CMD_TALK_TO_MOTOR:
               TalkToMotor(gbl_cur_ext_byte);
               break;
            case CMD_BURST_MODE:
               SetBurstMode(gbl_cur_ext_byte, gbl_cur_ext);
               break;
            case CMD_MISC_CONTROL:
               miscControl(gbl_cur_param, gbl_cur_ext, gbl_cur_ext_byte);
               break;
         }

         if (gbl_cur_cmd == CMD_READ_SENSOR)
            printf(usb_cdc_putc,"%c%c%c%c",ReplyHeader1, ReplyHeader2, SensorVal >> 8, SensorVal & 0xff);
         else {
            printf(usb_cdc_putc,"%c%c%c", ReplyHeader1, ReplyHeader2, ACK_BYTE);
            if (ping) { // Para garantir compatibilidade com o monitor de Stanford
               printf(usb_cdc_putc,"%c%c%c", 0x01, 0xF0, 0x01);
               ping = 0;
            }
         }

         if ((gbl_cur_cmd == CMD_MISC_CONTROL) && (gbl_cur_param == MISC_UPLOAD_EEPROM)) {
            uploadLen = ((((int16)gbl_cur_ext<<8) + gbl_cur_ext_byte)<<5);

            if (uploadLen == 0)
               uploadLen = read_program_eeprom(MEM_PTR_LOG_BASE_ADDRESS);

            uploadLen >>= 1;
            printf(usb_cdc_putc, "%c%c%c%c", EEPROMuploadHeader1, EEPROMuploadHeader2, uploadLen & 0xff, uploadLen >> 8);
            uploadLen <<= 1;

            for (counter = 0 ; counter < uploadLen ; counter++) {
               foo = read_program_eeprom((RECORD_BASE_ADDRESS + 2*counter));
               printf(usb_cdc_putc, "%c%c", foo&0xff, foo>>8);
               delay_ms(1);
            }
         }
         CMD_STATE = WAITING_FOR_FIRST_HEADER;
      } else {
         DoSensorStuff();
      }

      if (gblLogoIsRunning) {
         if (!gblWaitCounter){
                  evalOpcode(fetchNextOpcode());
         }
      }
      else if (gblTurnedOff) {
         init_variables();
         initBoard(0);
         gblTurnedOff = 0;
      }

      if (gblCmdTimeOut > CMD_TIMEOUT_PERIOD) {
         CMD_STATE = WAITING_FOR_FIRST_HEADER;
         gblCmdTimeOut = 0;
      }
   }
}

#include <logovm.c>
