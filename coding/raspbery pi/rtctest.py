import time
import rtc

r = rtc.RTC()
r.datetime = time.struct_time((2023, 1, 6, 13, 16, 14, 0, -1, -1))

rtc.set_time_source(r)
t = r.datetime

clock = str(t.tm_hour) + ":" + str(t.tm_min) + ":" + str(t.tm_sec)

while True:
    print(f"Kl. {time.time()}")
    time.sleep(1)