delphi-componente.txt 20/08/2011
unit ExampUnit;
(*
=====================================================================
NOTE Delphi 4 : BEFORE you run this example program, FIRST install
Compack.bpl
=====================================================================
From Delphi’s IDE :
Component > Install Packages > Add > select compack.bpl > OK
(TComport will be installed on the Samples page)
do *NOT* follow this procedure with D5 !
=====================================================================
NOTE Delphi 5 : BEFORE you run this example program, FIRST install
Comport.pas
=====================================================================
From Delphi’s IDE :
Component > Install > Into New Package
Unit file name : Browse to comport.pas > OK
Package File name : Browse to directory of comport.pas, type a name of
your choice, "ComPort" for example > OK
etc.. > OK etc.. OK
(TComport will be installed on the Samples page)
*)
interface
uses
Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
Comport, StdCtrls, ExtCtrls, Buttons;
type
TForm1 = class( TForm )
Memo1: TMemo;
Button6: TButton;
Button7: TButton;
Button8: TButton;
Button9: TButton;
Edit2: TEdit;
GroupBox1: TGroupBox;
shCarrier: TShape;
StaticText2: TStaticText;
shCTS: TShape;
shDSR: TShape;
StaticText3: TStaticText;
StaticText4: TStaticText;
shRing: TShape;
StaticText5: TStaticText;
shPortOpen: TShape;
StaticText6: TStaticText;
shError: TShape;
StaticText11: TStaticText;
cbPorts: TComboBox;
StaticText1: TStaticText;
ComPort1: TComPort;
SpeedButton1: TSpeedButton;
SpeedButton2: TSpeedButton;
SpeedButton3: TSpeedButton;
Image1: TImage;
procedure btnOpenClick( Sender: TObject );
procedure btnCloseClick( Sender: TObject );
procedure btnSendClick( Sender: TObject );
procedure Button6Click( Sender: TObject );
procedure Button7Click( Sender: TObject );
procedure Button8Click( Sender: TObject );
procedure Button9Click( Sender: TObject );
procedure FormCreate( Sender: TObject );
procedure FormDestroy( Sender: TObject );
1
delphi-componente.txt 20/08/2011
procedure ComPort1CTSChanged( Status: Boolean );
procedure ComPort1DSRChanged( Status: Boolean );
procedure ComPort1Error( Sender: TObject );
procedure ComPort1PortClose( Sender: TObject );
procedure ComPort1PortOpen( Sender: TObject );
procedure ComPort1RingChanged( Status: Boolean );
procedure ComPort1RLSDChanged( Status: Boolean );
procedure ComPort1ReceiveCallBack( Data: string );
procedure Edit2KeyPress( Sender: TObject; var Key: Char );
private
{ Private declarations }
public
{ Public declarations }
end;
var
Form1 : TForm1;
implementation
{$R *.DFM}
//==========================================
procedure TForm1.FormCreate( Sender: TObject );
begin
EnumPorts( cbPorts.Items );
if cbPorts.Items.Count > 0 then
cbPorts.ItemIndex := 0; // show first available port
end;
//===========================================
procedure TForm1.FormDestroy( Sender: TObject );
begin
ComPort1.Free;
end;
//============================================
procedure TForm1.btnOpenClick( Sender: TObject );
begin
cbPorts.Enabled := false;
ComPort1.Port := cbPorts.Items[cbPorts.ItemIndex];
ComPort1.Open;
end;
//=============================================
procedure TForm1.btnCloseClick( Sender: TObject );
begin
ComPort1.Close;
cbPorts.Enabled := true;
end;
//===================================================
procedure TForm1.ComPort1ReceiveCallBack( Data: string );
begin
if Memo1.Lines.Count > 500 then
Memo1.Clear;
Memo1.Lines.Add( Data );
Image1.Visible := not Image1.Visible;
end;
//=================================================
procedure TForm1.ComPort1CTSChanged( Status: Boolean );
begin
case Status of
true: shCTS.Brush.Color := clLIME;
false: shCTS.Brush.Color := clGRAY;
end;
end;
//==================================================
procedure TForm1.ComPort1DSRChanged( Status: Boolean );
begin
2
delphi-componente.txt 20/08/2011
case Status of
true: shDSR.Brush.Color := clLIME;
false: shDSR.Brush.Color := clGRAY;
end;
end;
//============================================
procedure TForm1.ComPort1Error( Sender: TObject );
begin
shError.Brush.Color := clRED;
end;
//===============================================
procedure TForm1.ComPort1PortOpen( Sender: TObject );
begin
shPortOpen.Brush.Color := clYELLOW;
end;
//================================================
procedure TForm1.ComPort1PortClose( Sender: TObject );
begin
shPortOpen.Brush.Color := clGRAY;
shCarrier.Brush.Color := clGRAY;
shRing.Brush.color := clGRAY;
shCTS.Brush.Color := clGRAY;
shDSR.brush.color := clGRAY;
end;
//==================================================
procedure TForm1.ComPort1RingChanged( Status: Boolean );
begin
case status of
true: shRing.Brush.Color := clLIME;
false: shRing.Brush.Color := clGRAY;
end;
end;
//===================================================
procedure TForm1.ComPort1RLSDChanged( Status: Boolean );
begin
case Status of
true: shCarrier.Brush.Color := clLIME;
false: shCarrier.Brush.Color := clGRAY;
end;
end;
//===========================================================
procedure TForm1.Edit2KeyPress( Sender: TObject; var Key: Char );
begin
if Key = #13 then
begin
BtnSendClick( Sender );
Key := #0;
end;
end;
//===========================================
procedure TForm1.btnSendClick( Sender: TObject );
begin
//ComPort1.send( edit2.Text + #13#10 );
ComPort1.send( #84 + #254 + #196 + #0 + #13#10 );
end;
//===========================================
procedure TForm1.Button6Click( Sender: TObject );
begin
ComPort1.SetRTS( true );
end;
//===========================================
procedure TForm1.Button7Click( Sender: TObject );
begin
ComPort1.SetRTS( false );
3
delphi-componente.txt 20/08/2011
end;
//===========================================
procedure TForm1.Button8Click( Sender: TObject );
begin
ComPort1.SetDTR( true );
end;
//============================================
procedure TForm1.Button9Click( Sender: TObject );
begin
ComPort1.SetDTR( false );
end;
end.
4
