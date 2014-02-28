#include <bios.h>
  unsigned temp;
    // Open serial port at 1200 baud, 8 data bits,
    //  No parity, 1 stop bit
  temp = _bios_serialcom(_COM_INIT, 0,
              _COM_CHR8 | _COM_NOPARITY | _COM_STOP1 | _COM_1200);
  temp = _bios_serialcom(_COM_RECEIVE, 0,  0);   // Read a character
  temp = _bios_serialcom(_COM_SEND   , 0, '*');  // Write a character
