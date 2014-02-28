import java.net.*;
import java.io.*;
public class MyDemo extends BApplet {
  Socket socket;
  PrintWriter out;
  BufferedReader in;
  void setup()
  {
    size(200, 200);
    background(255);
    try{
      socket=new Socket("192.168.5.101",9873 );
      in=new BufferedReader(
       new InputStreamReader(socket.getInputStream())
      );
      out=new PrintWriter(socket.getOutputStream());
    }catch(Exception e){println(e);}
  }
 
  void loop()
  {
    try{
     out.println("beep");
     out.flush();
    }catch(Exception e){println(e);}
  }
} 
