Public Class Form1

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        SerialPort1.PortName = "COM3"
        SerialPort1.Open()
        Dim data(4) As Byte
        data(0) = 84
        data(1) = 254
        data(2) = 196
        data(3) = 0
        SerialPort1.Write(data, 0, 4)
        SerialPort1.Close()
    End Sub
End Class
