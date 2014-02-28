SerPIC = serial('COM3');

set(SerPIC,'BaudRate',9600);

set(SerPIC,'DataBits',8);

set(SerPIC,'Parity','none');

set(SerPIC,'StopBits',1);

set(SerPIC,'FlowControl','none');

fopen(SerPIC);

fprintf(SerPIC,'%s','TþÄ^@');

fclose(SerPIC);

delete(SerPIC)

clear SerPIC
