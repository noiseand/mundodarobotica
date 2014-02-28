Private Sub Command2_Click()
'Envia o beep para a porta COMM
    MSComm1.Output = Chr(84) + Chr(254) + Chr(196) + Chr(0)
End Sub

Private Sub Form_Load()
    MSComm1.CommPort = 6
    MSComm1.Settings = "9600,n,8,1"
    MSComm1.InputLen = 0
    MSComm1.PortOpen = True
End Sub


Private Sub Form_Unload(Cancel As Integer)
    MSComm1.PortOpen = False
End Sub
