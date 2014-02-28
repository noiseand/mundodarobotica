using System.IO;  
using System.IO.Ports;   
 
public class SerialCom  
{  
 
    private string portName = null;  
    private List<byte> recordInList;  
 
    public boolean bgotSerialComMessage = false;  
 
    public SerialCom(object sender)  
    {  
        sring[] portsList = SerialPort.GetPortNames(); /* Get a list of serial port names*/ 
        ArrayList pList = new ArrayList();   
 
        /*find an appropriate com port to open*/ 
        foreach (string port in portsList)  
        {  
            pList.Add(port.ToUpper());  
        }    
 
        pList.Sort();  
          
        foreach (string port in pList)  
        {  
            try 
            {  
                sp = new SerialPort(port, 9600, Parity.None, 8, StopBits.One);  
                sp.NewLine = "\r";  
                sp.Encoding = Encoding.GetEncoding(20127);  
                sp.Open();  
                sp.DataReceived += new SerialDataReceivedEventHandler(sp_DataReceived);  
                this.portName = port;  
                recordInList = new List<byte>();  
                break;  
            }  
            catch 
            {  
                throw new Exception("no port available");  
            }  
        }//end foreach  
    }//end constructor  
   
 
    public void UnRegisterSerialComHost()  
    {  
        sp.DataReceived -= new SerialDataReceivedEventHandler(sp_DataReceived);  
    }//end method   
   
 
    public void RegisterSerialComHost()  
    {  
        sp.DataReceived += new SerialDataReceivedEventHandler(sp_DataReceived);  
    }//end method   
   
 
    public void WriteBytes(List<byte> bytelist)  
    {  
        byte[] b;  
        b = bytelist.ToArray();  
        sp.Write(b, 0, b.Length);  
    }//end method   
   
 
    /*write a character to com port*/ 
    public void WriteChar(char c)  
    {  
        string s = c.ToString();  
        try 
        {  
            sp.Write(s);  
        }  
        catch (InvalidOperationException)  
        {  
            throw new Exception("failed");  
        }  
    }//end method   
   
 
    /*write a string to com port*/ 
    public void WriteString(string s)  
    {  
        try 
        {  
            sp.Write(s);  
        }     
        catch (InvalidOperationException)  
        {  
            throw new Exception("failed");  
        }  
    }//end method  
   
 
    private void sp_DataReceived(object sender, SerialDataReceivedEventArgs e)  
    {  
        byte recChar;  
        if (!sp.IsOpen)  
        {  
            return;  
        }  
        else 
        {  
            try 
            {  
                /*receiving a message*/ 
                while (sp.BytesToRead > 0)  
                {  
                    recChar = (byte)sp.ReadByte();  
                    if (recCha != ConstantData.ACK)  
                    {  
                        recordInList.Add(recChar);  
                    }  
                    else 
                    {  
                        /*got an ACK -> end of response*/ 
                        recordInList.Add(recChar);  
                        bgotSerialComMessage = true;  
                    }  
                }//end while  
            }  
            catch (SystemException ex)  
            {  
                Console.WriteLine("SerialCom data receive error {0}{1}", ex.Message, ex.StackTrace);  
            }//end try-catch  
        }//end if-else  
    }//end method (event handler)   
 
 
}//end class 
