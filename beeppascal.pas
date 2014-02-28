program sample; { Dumb Terminal }

uses async,crt;

var
   c: char;
  { com1: com_port;{}
   com3: com_port;

begin
  com3.init(1,10000,10000);            {set up the buffers & interrupt    }
  com3.setparam(9600,8,'N',1);        {set up the baud,ws,parity,&stopbts}
  com3.enable;                         {enable the com port               }
  c:= ' ';
              {initialize the scratch variable   }
  ClrScr;
  TextColor(15);
  writeln('Dumb terminal version 0.1');
  TextColor(10);
  writeln('Use crossover cable between COM1 ports of two PCs:');
  writeln('     2----3  Use DB9F for connectors. ');
  writeln('     3----2');
  writeln('     5----5');
  TextColor(15);
  writeln('Press ESC to exit program:');
  repeat
    if keypressed then begin
      c:= readkey;
      textcolor(9);
      write(c);
      if ord(c)=13 then writeln;
      if c <> #27 then com3.write(c);
    end;
   if com3.waiting then begin
     textcolor(14);
     write(com3.read);
   end;
  until c = #27;
  com3.done;
  textcolor(7);
end.
