set o = CreateObject("GoGoX28.GoGoXctl28")

o.CommPort=3
o.Initialize
o.beep
o.CloseComm

WScript.Echo "beep!"
