<?php

$fp = fopen("/dev/ttyACM0", "w");

fwrite($fp, chr(84));
fwrite($fp, chr(254));
fwrite($fp, chr(196));
fwrite($fp, chr(0));
fwrite($fp, chr(13));
fwrite($fp, chr(10));

fclose($fp);

?>
