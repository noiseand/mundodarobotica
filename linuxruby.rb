require 'rubygems'
require 'serialport' # use Kernel::require on windows, works better.

#params for serial port
port_str = "/dev/ttyACM0" #may be different for you
baud_rate = 9600
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE

sp = SerialPort.new(port_str, baud_rate, data_bits, stop_bits, parity)

i = 84
sp.write(i.chr)

i = 254
sp.write(i.chr)

i = 196
sp.write(i.chr)

i = 0
sp.write(i.chr)

i = 13
sp.write(i.chr)

i = 10
sp.write(i.chr)
