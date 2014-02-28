S=new Stream(‘com3://9600');

S.write(‘\x54');
S.write(‘\xFE’);
S.write(‘\xC4');
S.write(‘\x00');
S.write(‘\r’);
S.write(‘\n’);
