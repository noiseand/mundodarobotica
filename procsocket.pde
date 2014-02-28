void setup()
{
size(400,300);
background(#FFCC00);
fill(0);
noStroke();
}

void draw()
{
scale(1.3);
try
{
String host = "127.0.0.1"; // change

text( "Connecting to " + host + " ...", 10, 20 );
Socket s = new Socket( host, 9873 );

text( "Connection established to " + s.getInetAddress(), 10, 30 );

BufferedReader in = new BufferedReader( new InputStreamReader( s.getInputStream() ) );
PrintStream out = new PrintStream( s.getOutputStream() );

int lineNum = 50;

out.println( "beep" );
out.println( "stop" );
out.flush();

String t = in.readLine();
System.out.println( t );

fill(0);
text("Ping Done.", 10, lineNum+10);

try
{
Thread.sleep( 1000 );
} catch ( Exception e )
{
}

in.close();
out.close();
}
catch ( Exception e )
{
println( "Error" + e );
}
}
