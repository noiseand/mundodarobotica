// Program Name: ArduinoBoard
// Version: 0.6 
// Author: Rai and Mike McKenzie
// Description: A program that simulates a Scratch board using 
//              an Arduino. This version reads the button and 
//              inputs A-D.
// 
//
// Based on Relay Wii Nunchuck status to Scratch
// By Mike Donovan
//

//============================================================================

#include <Wire.h>

int BYTE;

void ScratchBoardSensorReport(int sensor, int value)
{
  Serial.print( B10000000 | ((sensor & B1111)<<3) | ((value>>7) & B111), BYTE);
  Serial.print( value & B1111111, BYTE);
}

void setup()
{
  Serial.begin(38400);
  pinMode(4, INPUT);
}

void loop() 
{ 

// Format output for ScratchBoard emulation
// sensor=0-7, value=0-1023 (rescaled by Scratch to 0-100)
// 0="A", 1="B", 2="C", 3="D",
// 4="Slider", 5="Light", 6="Sound", 7="Button"

  ScratchBoardSensorReport(0, analogRead(1));
  ScratchBoardSensorReport(1, analogRead(2));
  ScratchBoardSensorReport(2, analogRead(3));
  ScratchBoardSensorReport(3, analogRead(4));
  ScratchBoardSensorReport(4, 0);
  ScratchBoardSensorReport(5, 0);
  ScratchBoardSensorReport(6, 0);
  ScratchBoardSensorReport(7, digitalRead(4)?1023:0);
  // Let Scratch catch up with us
  delay(30);
}
