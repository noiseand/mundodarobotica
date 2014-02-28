@echo off

sdcc -mpic16 -p18f4550 --mplab-comp -c main.c --code-loc 0x1840
sdcc -mpic16 -p18f4550 --code-loc 0x1840 --no-crt main.o libc18f.lib libm18f.lib libio18f4550.lib -m -o main.hex
pause
