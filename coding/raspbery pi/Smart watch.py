import time
import digitalio
import board
import busio
import usb_hid
import random
import displayio
import terminalio
import microcontroller
import adafruit_displayio_ssd1306
from adafruit_display_text import label
from adafruit_display_shapes.line import Line
import gc
from adafruit_bitmap_font import bitmap_font
import rtc

temperature = 0

r = rtc.RTC()
r.datetime = time.struct_time((2023, 1, 6, 13, 16, 14, 0, -1, -1))

rtc.set_time_source(r)
t = r.datetime

# encSw2 = digitalio.DigitalInOut(board.GP21)
encSw2 = digitalio.DigitalInOut(board.GP21)
encSw2.direction = digitalio.Direction.INPUT
encSw2.pull = digitalio.Pull.UP

font_file = "fonts/LeagueSpartan-Bold-16.bdf"
font = bitmap_font.load_font(font_file)

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

def cont_gen():
    global group
    del group

    temp_string = "Kl. ho:mi:se"
    temp_show = label.Label(
        font, text=temp_string, color=0xFFFF00, x=5, y=15
    )

    group = displayio.Group()
    
    group.insert(0, Line(0, 60, 127, 60, 0xFFFFFF))
    
    group.insert(1, temp_show)
    display.show(group)

def temp_up():
    global group

    time = str(t.tm_hour) + ":" + str(t.tm_min) + ":" + str(t.tm_sec)
    temp_string = f"Kl. {time}"
    temp_show = label.Label(
        font, text=temp_string, color=0xFFFF00, x=5, y=15
    )

    group.pop(1)
    group.insert(1, temp_show)
    display.show(group)

# Displaying info
display.show(group)
cont_gen()

# loop
while True:
    temp_up()
    # poll encoder button 2
    if encSw2.value == 0:
        print("enc switch 2 pressed and showed message!")

        temp_up()
    time.sleep(1)