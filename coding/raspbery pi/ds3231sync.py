import time
from ds3231_gen import *

i2c = machine.SoftI2C(scl=machine.Pin(9, machine.Pin.OPEN_DRAIN, value=1), sda=machine.Pin(8, machine.Pin.OPEN_DRAIN, value=1))

d = DS3231(i2c)
rtc = machine.RTC()

d.set_time(tt=None)