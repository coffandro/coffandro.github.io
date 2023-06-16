import time
import digitalio
import board
import busio
import rotaryio
import usb_hid
import random
import displayio
import terminalio
import microcontroller
from adafruit_hid.consumer_control import ConsumerControl
from adafruit_hid.consumer_control_code import ConsumerControlCode
from adafruit_hid.keyboard import Keyboard
from adafruit_hid.keycode import Keycode
import adafruit_displayio_ssd1306
from adafruit_display_text import label
from adafruit_display_shapes.line import Line
import gc

temperature = 0

# The display Messages
Messages = [
    "Fix your posture \nyou fucking shrimp!",
    "Better late than \nnever, idiot!",
    "Remember to take \nbreaks, fuckhead!",
    "Drink some coffee \nyou addicted fuck!",
    "Get back to work \nyou lazy dick!",
    "Your lack of \ndedication is an \ninsult",
]

# Rotary encoder
enc = rotaryio.IncrementalEncoder(board.GP16, board.GP17)
encSw = digitalio.DigitalInOut(board.GP20)
encSw.direction = digitalio.Direction.INPUT
encSw.pull = digitalio.Pull.UP
lastPosition = 0

# Secondary encoder
enc2 = rotaryio.IncrementalEncoder(board.GP18, board.GP19)
encSw2 = digitalio.DigitalInOut(board.GP21)
encSw2.direction = digitalio.Direction.INPUT
encSw2.pull = digitalio.Pull.UP
lastPosition2 = 0

# builtin LED
led = digitalio.DigitalInOut(board.GP25)
led.direction = digitalio.Direction.OUTPUT

# USB device
consumer = ConsumerControl(usb_hid.devices)

# Initialize Keyboard
kbd = Keyboard(usb_hid.devices)

# button delay
dl = 0.2

# Setting up the display
displayio.release_displays()

sda, scl = board.GP4, board.GP5

i2c = busio.I2C(scl, sda)
display_bus = displayio.I2CDisplay(i2c, device_address=0x3C)
display = adafruit_displayio_ssd1306.SSD1306(display_bus, width=128, height=64)

# Make the display context
bitmap = displayio.Bitmap(128, 64, 1)
palette = displayio.Palette(1)
palette[0] = 0xFFFFFF  # White

# Making a group
group = displayio.Group()

text = random.choice(Messages)
text_show = label.Label(terminalio.FONT, text=text, color=0xFFFF00, x=0, y=5)


def text_gen():
    global text_show
    global group
    del group

    temp1 = round(microcontroller.cpu.temperature, 0)
    temp2 = round(microcontroller.cpu.temperature, 1)
    temps = [temp1, temp2]
    temp_string = f"CPU {sum(temps)/len(temps)}°C"
    temp_show = label.Label(
        terminalio.FONT, text=temp_string, color=0xFFFF00, x=75, y=40
    )

    group = displayio.Group()
    group.insert(0, text_show)
    group.insert(0, temp_show)
    group.insert(1, Line(0, 50, 127, 50, 0xFFFFFF))
    group.insert(2, Line(0, 50, 4, 54, 0xFFFFFF))
    group.insert(1, Line(2, 52, 125, 52, 0xFFFFFF))
    group.insert(2, Line(127, 50, 123, 54, 0xFFFFFF))
    group.insert(1, Line(4, 54, 123, 54, 0xFFFFFF))
    display.show(group)


# Displaying info
display.show(group)

# loop
while True:
    text_gen()
    # poll encoder position
    position = enc.position
    if position != lastPosition:
        led.value = True
        if lastPosition < position:
            print("Volume increased!")
            consumer.send(ConsumerControlCode.VOLUME_INCREMENT)
        else:
            print("Volume decreased!")
            consumer.send(ConsumerControlCode.VOLUME_DECREMENT)
        lastPosition = position
        led.value = False

        # poll encoder2 position
    position2 = enc2.position
    if position2 != lastPosition2:
        led.value = True
        if lastPosition2 < position2:
            print("Brightness increased!")
            consumer.send(ConsumerControlCode.BRIGHTNESS_INCREMENT)
        else:
            print("Brightness decreased!")
            consumer.send(ConsumerControlCode.BRIGHTNESS_DECREMENT)
        lastPosition2 = position2
        led.value = False

    # poll encoder button
    if encSw.value == 0:
        print("enc switch 1 pressed!")
        consumer.send(ConsumerControlCode.PLAY_PAUSE)
        led.value = True
        time.sleep(dl)
        led.value = False

    # poll encoder button 2
    if encSw2.value == 0:
        print("enc switch 2 pressed and showed message!")

        text = random.choice(Messages)
        text_show = label.Label(terminalio.FONT, text=text, color=0xFFFF00, x=0, y=5)

        text_gen()

        temp1 = round(microcontroller.cpu.temperature, 0)
        temp2 = round(microcontroller.cpu.temperature, 1)
        temp1_string = f"CPU 1 {temp1}°C"
        temp2_string = f"CPU 2 {temp2}°C"
        print(temp1_string)
        print(temp2_string)

        print(gc.mem_free())

        led.value = True
        time.sleep(dl)
        led.value = False

    time.sleep(0.1)
