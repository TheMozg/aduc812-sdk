@echo off
powershell -Command "& { $port= new-Object System.IO.Ports.SerialPort COM8,2400,None,8,one; $port.open( ); for($i=1; $i -le \"%2\"; $i++){ $port.ReadLine( ) }; $port.Close( ); }"