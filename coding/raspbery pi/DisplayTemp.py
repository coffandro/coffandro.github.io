import displayio, board, busio
import adafruit_displayio_ssd1306
from adafruit_display_text import label
from adafruit_display_shapes.line import Line
import terminalio

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

# Draw a label
text = "Streak"
text_show = label.Label(terminalio.FONT, text=text, color=0xFFFF00, x=20, y=20)


# Displaying info
group.append(text_show)
group.append(Line(16, 50, 112, 50, 0xFFFFFF))
display.show(group)

while True:
    pass