OpenI2C( SLAVE_7, SLEW_OFF);
SSPADD = 0xB0; //SSPADD contains I2C device address in SLAVE mode

while ( !DataRdyI2C() )
{
addr = ReadI2C();
AckI2C();
IdleI2C();
data = ReadI2C();
NAckI2C();
IdleI2C();
}
