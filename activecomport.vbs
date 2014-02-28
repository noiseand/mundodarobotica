Set objComport = CreateObject( "ActiveXperts.Comport" ) ' Create a new Comport instance

objComport.Device = "COM3" ' Use a COM port directly (no Windows Device Driver)
objComport.BaudRate = 9600 ' Set baudrate (default value: 9600)
objComport.HardwareFlowControl = asFLOWCONTROL_ENABLE ' Set Hardware Flow Control (default: On)
objComport.SoftwareFlowControl = asFLOWCONTROL_ENABLE ' Set Software Flow Control (default: Off)
objComport.Open ' Open the port
Wscript.Echo "Open, result: " & objComport.LastError
If( objComport.LastError 0 ) Then
WScript.Quit
End If

objComport.WriteString( "TþÄ^@" ) ' Write command
str = objComport.ReadString
WScript.Echo "Received: [" & str & "]" ' Read the response

objComport.Close
