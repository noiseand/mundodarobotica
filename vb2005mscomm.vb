Private Sub Form1_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
    MSComm1.CommPort = 6
    MSComm1.Settings = "9600,n,8,1"
    MSComm1.InputLen = 0
End Sub

Private Sub Command1_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Command1.Click
    MSComm1.PortOpen = True
End Sub

Private Sub Command2_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Command2.Click
'Envia o beep para a porta COMM
    MSComm1.Output = Chr(84) & Chr(254) & Chr(196) & Chr(0)
End Sub

Private Sub Form1_FormClosed(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.FormClosedEventArgs) Handles Me.FormClosed
   MSComm1.PortOpen = False
End Sub
