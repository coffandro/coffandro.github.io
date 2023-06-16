import time
import digitalio
import board
import rotaryio
import usb_hid
from adafruit_hid.consumer_control import ConsumerControl
from adafruit_hid.consumer_control_code import ConsumerControlCode


from adafruit_hid.keycode import Keycode
from adafruit_hid.keyboard import Keyboard

keyboard.send(Keycode.ONE)
time.sleep(0.1)
keyboard.release(Keycode.ONE)
# Rotary encoder
enc = rotaryio.IncrementalEncoder(board.GP13, board.GP14)
encSw = digitalio.DigitalInOut(board.GP15)
encSw.direction = digitalio.Direction.INPUT
encSw.pull = digitalio.Pull.UP
lastPosition = 0

# builtin LED
led = digitalio.DigitalInOut(board.GP25)
led.direction = digitalio.Direction.OUTPUT

# USB device
consumer = ConsumerControl(usb_hid.devices)

# button delay
dl = 0.2

# loop
while True:
    # poll encoder position
    position = enc.position
    if position != lastPosition:
        led.value = True
        if lastPosition < position:
            consumer.send(ConsumerControlCode.VOLUME_INCREMENT)
        else:
            consumer.send(ConsumerControlCode.VOLUME_DECREMENT)
        lastPosition = position
        led.value = False
        
    # poll encoder button
    if encSw.value == 0:
        print("Encoder Switch pressed")
        consumer.send(ConsumerControlCode.PLAY_PAUSE)
        time.sleep(dl)
        
    time.sleep(0.1)