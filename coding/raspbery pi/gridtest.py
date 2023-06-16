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
import 

temperature = 0

# The display Messages
Messages = [
            "Fix your posture \nyou fucking shrimp!",
            "Better late than \nnever, idiot!",
            "Remember to take \nbreaks, fuckhead!",
            "Drink some coffee \nyou addicted fuck!",
            "Get back to work \nyou lazy dick!",
            "Your lack of \ndedication is an \ninsult",
            "I don't need \ninspiration quotes \ni need coffee",
            "I run entirely\non Caffeine and \nsarcasm"
            ]

# Setting up the display
displayio.release_displays()

sda, scl = board.GP4, board.GP5
    
i2c = busio.I2C(scl, sda)
display_bus = displayio.I2CDisplay(i2c, device_address=0x3C)
display = adafruit_displayio_ssd1306.SSD1306(display_bus, width=128, height=64)

# Make the display context
splash = displayio.Group()

color_bitmap = displayio.Bitmap(128, 64, 1)
color_palette = displayio.Palette(1)
color_palette[0] = 0x000000  # White

# Draw a label
text = random.choice(Messages)
text_area = label.Label(terminalio.FONT, text=text, color=0xFFFF00, x=10, y=10)

display.show(text_area)

temperature_string = f"{temperature}°C"
temperature_value_label = label.Label(terminalio.FONT, text = temperature_string, color = 0xFFFFFF)
temperature_value_label.anchor_point = (1, 1) # Change anchor point to center
temperature_value_label.anchored_position = (120, 60)

display.show(temperature_value_label)

# loop
while True:
        display.show(text_area)
        led.value = True
        time.sleep(dl)
        led.value = False
        
    time.sleep(0.1)
