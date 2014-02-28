# Set up the serial port
use Device::SerialPort;
my $port = Device::SerialPort->new("/dev/ttyACM0");

# 9600, 81N on the USB ftdi driver
$port->baudrate(9600);
$port->databits(8);
$port->parity("none");
$port->stopbits(1);

$port->write("\x54\xFE\xC4\x00\r\n");

$port->close();
