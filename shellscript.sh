stty -F /dev/ttyACM0 9600

echo -e '\x54\xFE\xC4\x00' > /dev/ttyACM0
