# -*- coding: utf-8 -*-
import serial
import os, glob

SEND_COMMAND_CODE = 84, 254
ACKNOWLEDGE_CODE = 85, 255, 170

#2 bytes commands:
BEEP_CODE = 196, 0

#========================================================================================================#


if __name__ == '__main__':
    comunicSerial = serial.Serial(None, 9600, bytesize=serial.EIGHTBITS, \
                                                       parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, \
                                                   timeout=0.1, xonxoff=0, rtscts=0)
    comunicSerial.port = 2
    comunicSerial.open()
    command = SEND_COMMAND_CODE + BEEP_CODE
    s = ""
    for i in command:
        s = s + chr(i)
    comunicSerial.write(s)
