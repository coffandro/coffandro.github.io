import supervisor

while True:
    if supervisor.runtime.serial_bytes_available:
        value = input().strip()
        print(f"Received: {value}\r") 
