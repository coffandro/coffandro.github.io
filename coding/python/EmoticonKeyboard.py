import pyautogui
import os
import subprocess
from tkinter import *
from tkinter import ttk
import tkinter

Emojis = ["-_-", "0_0", "¯\_(ツ)_/¯"]
Width = 3

#setup of functions
root = Tk()
frm = ttk.Frame(root, padding=1)
frm.grid()
root.title("EmotiKeyboard")

def exit(event):
    root.destroy()

for i in Emojis:
    ttk.Button(frm, text=i, command=exit).pack()

#Automatically close the window after 3 seconds
root.after(10000,lambda:root.destroy())

#hotkeys which are not working at the moment, please return to this later
root.bind("<Escape>", exit)

#displaying objects
root.mainloop()