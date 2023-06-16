#!/usr/bin/env python3
#
# Vendor:Product ID for Raspberry Pi Pico is 2E8A:0005
#
from serial.tools import list_ports
import serial, time

picoPorts = list(list_ports.grep("2E8A:0005"))
if not picoPorts:
    print("No Raspberry Pi Pico found")
else:
    picoSerialPort = picoPorts[0].device
    with serial.Serial(picoSerialPort) as s:
        syncMSG = 'T'+str( int(time.time()) )
        s.write(bytes(syncMSG, "ascii"))
    with serial.Serial(picoSerialPort) as s:
        syncMSG = 'T'+str( int(time.time()) )
        s.write(bytes(syncMSG, "ascii"))
    print( "Raspberry Pi Pico found at "+str(picoSerialPort) )
    print( "Time sync epoch USB MSG: "+syncMSG )
