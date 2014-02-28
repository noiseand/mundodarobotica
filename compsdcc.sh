#sdcc --ivt-loc=0x1840 -mpic16 -p18f4550 -c main.c
sdcc --ivt-loc=0x1840 -c --use-non-free -mpic16 -p18f4550 main.c

sdcc -mpic16 -p18f4550 --ivt-loc=0x1840 --no-crt main.o libc18f.lib libm18f.lib libio18f4550.lib -m -o main.hex
