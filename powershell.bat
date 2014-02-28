@echo off

powershell "$port= new-Object System.IO.Ports.SerialPort COM3,9600,None,8,one; $port.open(); $port.Write("TþÄ^@"); $port.Close()"

pause
