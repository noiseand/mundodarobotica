OpenI2C( MASTER, SLEW_OFF);
SSPADD = 0x27; //SSPADD Baud Register used to calculate I2C clock speed in MASTER mode (in this case 100Khz)

StartI2C();
IdleI2C();
putcI2C( 0xB0 ); //send address
IdleI2C();
putcI2C( databyte ); //send data
IdleI2C();
StopI2C();
