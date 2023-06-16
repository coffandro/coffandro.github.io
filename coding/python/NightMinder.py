import os
import subprocess
from tkinter import *
from tkinter import ttk

#setup of functions
root = Tk()
frm = ttk.Frame(root, padding=1)
frm.grid()
root.title("NightMinder")

def TurnOn():
    os.system("gsettings set org.gnome.settings-daemon.plugins.color night-light-enabled true")
    ttk.Label(frm, text=f"Turned Night light on", padding=10,borderwidth=3, relief="sunken").pack()
def TurnOff():
    os.system("gsettings set org.gnome.settings-daemon.plugins.color night-light-enabled false")
    ttk.Label(frm, text=f"Turned Night light off", padding=10,borderwidth=3, relief="sunken").pack()
def Close():
    root.destroy()
def exit(event):
    root.destroy()

#making objects
ttk.Label(frm, text="Please choose an option").pack()
ttk.Label(frm, text=f"for your Night Light Status").pack()

ttk.Button(frm, text="Night Light On", command=TurnOn).pack()
ttk.Button(frm, text="Night Light Off", command=TurnOff).pack()
ttk.Button(frm, text="Quit", command=Close).pack()

#Automatically close the window after 3 seconds
root.after(10000,lambda:root.destroy())

#hotkeys which are not working at the moment, please return to this later
root.bind("<Escape>", exit)

#displaying objects
root.mainloop()