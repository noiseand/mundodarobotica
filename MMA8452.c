//// acelerometro

#define  I2C_WRITE_ADDR 0x3A
#define  I2C_READ_ADDR  0x3B

//define register addresses of MMA8452.
#define CTRL_REG1 0x2A
#define PL_CFG 0x11
#define PL_BF_ZCOMP 0x13
#define CTRL_REG4 0x2D
#define CTRL_REG5 0x2E
#define CTRL_REG2 0x2B
#define PL_COUNT 0x12
#define CTRL_REG3 0x2C
#define STATUS 0x00
#define INT_SOURCE 0x0C
#define PL_STATUS 0x10
#define OUT_X_MSB 0x01

#define FREAD_MASK 0x02
#define ACTIVE_MASK 0x01

#define OUT_X_MSB_REG 0x01
#define OUT_X_LSB_REG 0x02

#define OUT_Y_MSB_REG 0x03
#define OUT_Y_LSB_REG 0x04

#define OUT_Z_MSB_REG 0x05
#define OUT_Z_LSB_REG 0x06

int8 readreg(int8 address)
{
int8 data;
 i2c_start();                // initiate start condition
  i2c_write(I2C_WRITE_ADDR);            // device address
  i2c_write(address);            // register address
  i2c_start();
  i2c_write(I2C_READ_ADDR);            // device address
  data = i2c_read(0);
  i2c_stop();
return data;

}
void writeReg(unsigned char address, unsigned char data)
{
  i2c_start();
  i2c_write(I2C_WRITE_ADDR);//
  i2c_write(address);   //
  i2c_write(data);
  i2c_stop();
}
void MMA8452_Standby (void)
{
byte n;
n = readreg(CTRL_REG1);
writeReg(CTRL_REG1, n & ~ACTIVE_MASK);
}

void MMA8452_Active ()
{

writeReg(CTRL_REG1, (readreg(CTRL_REG1) | ACTIVE_MASK));
}

////
