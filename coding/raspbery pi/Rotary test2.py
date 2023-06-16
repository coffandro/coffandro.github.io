import board
import time
import usb_hid
from digitalio import DigitalInOut, Direction, Pull
from rotaryio import IncrementalEncoder
from adafruit_hid.consumer_control_code import ConsumerControlCode
from adafruit_hid.consumer_control import ConsumerControl

# Change the below code for different outcomes
# https://circuitpython.readthedocs.io/projects/hid/en/latest/

# Button Press will mute
BUTTON_CODE = ConsumerControlCode.PLAY_PAUSE

# Rotating the encoder clockwise will increase the volume
INCREMENT_CODE = ConsumerControlCode.VOLUME_INCREMENT

# Rotating the encoder clockwise will decrease teh volume
DECREMENT_CODE = ConsumerControlCode.VOLUME_DECREMENT

# initialize as hid device
consumer_control = ConsumerControl(usb_hid.devices)

# initialize encoder on pins D0 and D1 (QT Py M0)
encoder = IncrementalEncoder(board.GP17, board.GP16)

# initialize encoder click on pin D2 (QT Py M0)
button = DigitalInOut(board.GP20)
button.direction = Direction.INPUT
button.pull = Pull.UP

button_in = False
last_position = None

while True:

    if not button.value and not button_in:
        print("button press")
        button_in = True
        consumer_control.send(BUTTON_CODE)
        time.sleep(.2)

    elif button.value and button_in:
        button_in = False

    elif button.value and not button_in:
        position = encoder.position

        if last_position is not None and position != last_position:

            if position > last_position:
                print("rotate clockwise")
                consumer_control.send(INCREMENT_CODE)

            elif position < last_position:
                print("rotate counter-clockwise")
                consumer_control.send(DECREMENT_CODE)

        last_position = position