wserial=io.open("/dev/ttyACM0","w")
wserial:write("Serial Port OK")
wserial:flush()

rserial=io.open("/dev/ttyACM0","r")

wserial:write(string.format("\84\254\196\n"))
