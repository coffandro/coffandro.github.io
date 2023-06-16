import matplotlib
matplotlib.use("TkAgg")
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure

from tkinter import *
import customtkinter
from time import strftime

root = customtkinter.CTk()
photo = PhotoImage(file = '/home/coffandro/Downloads/Icon.png')
root.iconphoto(False, photo)
root.geometry("480x320")
root.title("")

AdvClock = False
FONT = ('calibri', 35, 'bold')
BTNSIZE = 75
PADDING = 10
def Test():
    print("Hello World!")
def SecFunc():
    global AdvClock
    AdvClock = not AdvClock
    return AdvClock
    time()
def time():
    if AdvClock == True:
        string = strftime('%H:%M:%S %p')
    else:
        string = strftime('%H:%M %p')
    lbl.configure(text=string)
    lbl.after(1000, time)

NEBTNs = customtkinter.CTkFrame(root, width=210, height=300,)

lbl = customtkinter.CTkLabel(root, font=FONT, width=270, height=30)

AdvBtn = customtkinter.CTkButton(NEBTNs, 
                                 text=f"sec", 
                                 width=BTNSIZE, height=BTNSIZE, 
                                 command=Test,
                                 font=FONT)
AdvBtn2 = customtkinter.CTkButton(NEBTNs, 
                                 text=f"sec", 
                                 width=BTNSIZE, height=BTNSIZE, 
                                 command=Test, 
                                 font=FONT)
AdvBtn3 = customtkinter.CTkButton(NEBTNs, 
                                 text=f"sec", 
                                 width=BTNSIZE, height=BTNSIZE, 
                                 command=Test, 
                                 font=FONT)
# second column
AdvBtn4 = customtkinter.CTkButton(NEBTNs,
                                 text=f"sec", 
                                 width=BTNSIZE, height=BTNSIZE, 
                                 command=SecFunc, 
                                 font=FONT)
AdvBtn5 = customtkinter.CTkButton(NEBTNs, 
                                 text=f"sec", 
                                 width=BTNSIZE, height=BTNSIZE, 
                                 command=Test, 
                                 font=FONT)
AdvBtn6 = customtkinter.CTkButton(NEBTNs, 
                                 text=f"sec", 
                                 width=BTNSIZE, height=BTNSIZE, 
                                 command=Test, 
                                 font=FONT)


f = Figure(figsize=(1,1), dpi=100)
a = f.add_subplot(111)
a.plot([1,2,3,4,5,6,7,8],[5,6,1,3,8,9,3,5])

lbl.pack(anchor='nw')

NEBTNs.pack(anchor='ne')
AdvBtn.grid(row = 0, column = 0, padx=PADDING, pady=PADDING)
AdvBtn2.grid(row = 1, column = 0, padx=PADDING, pady=PADDING)
AdvBtn3.grid(row = 2, column = 0, padx=PADDING, pady=PADDING)
AdvBtn4.grid(row = 0, column = 1, padx=PADDING, pady=PADDING)
AdvBtn5.grid(row = 1, column = 1, padx=PADDING, pady=PADDING)
AdvBtn6.grid(row = 2, column = 1, padx=PADDING, pady=PADDING)
canvas = FigureCanvasTkAgg(f)
canvas.draw()
canvas.get_tk_widget().pack(anchor="w")

time()

mainloop()