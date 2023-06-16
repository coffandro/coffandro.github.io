import time
import serial
ser = serial.Serial('/dev/serial//ttyUSB0', 115200)  # open serial port

command = b'hello\n\r'
print(f"Sending Command: [{command}]")
ser.write(command)     # write a string

ended = False
reply = b''

for _ in range(len(command)):
    a = ser.read() # Read the loopback chars and ignore

while True:
    a = ser.read()

    if a== b'\r':
        break

    else:
        reply += a

    time.sleep(0.01)

print(f"Reply was: [{reply}]")

ser.close()