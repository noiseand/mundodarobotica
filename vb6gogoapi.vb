Private Declare Sub beep Lib "GoGoBurst" ()
Private Declare Sub connect Lib "GoGoBurst" (ByVal ComPort As Integer)
'Private Declare Sub close Lib "GoGoBurst" ()

Private Sub Command3_Click()
    beep
End Sub

Private Sub Command2_Click()
    connect (3)
End Sub

