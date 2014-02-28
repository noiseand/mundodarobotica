     1: #include<htc.h>
     2: 
     3: void main()
     4: {
     5: 
     6: TRISB = 0b00000011;
     7: TRISA = 0b00101111;
     8: TRISC = 0b00000000;
     9: TRISD = 0b00000010;
    10: TRISE = 0b00000111;
    11: 
    12: while (1){
    13:         RC2 = 1;
    14:         _delay(100000);
    15:         RC2 = 0;
    16:         _delay(100000);
    17: }
    18: 
    19: }
