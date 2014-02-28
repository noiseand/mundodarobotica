"@echo off

"C:\MCC18\bin\mcc18.exe" -p=18F4550 "main.c" -fo="main.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-
"C:\MCC18\bin\mplink.exe" /p18F4550 "main.o" /u_CRUNTIME /u_DEBUG /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /o"main.cof" /M"main.map" /W
pause"
