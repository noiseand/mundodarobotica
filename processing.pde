// Example by Tom Igoe

import processing.serial.*;

// The serial port:
Serial myPort;

// List all the available serial ports:
println(Serial.list());

// I know that the first port in the serial list on my mac
// is always my Keyspan adaptor, so I open Serial.list()[0].
// Open whatever port is the one you're using.
myPort = new Serial(this, Serial.list()[1], 9600);

// Send a data out the serial port
myPort.write(84);
myPort.write(254);
myPort.write(196);
myPort.write(0);
myPort.close(0);
