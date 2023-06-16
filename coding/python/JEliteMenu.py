#importing libraries
from distutils.log import info
import os
import subprocess
from tkinter import *
from tkinter import ttk
import customtkinter
import importlib

#setup of variables
DeviceName="Jabra ELite 3"
DeviceMacAdd="2C:E0:32:A5:D4:26"

#setup of functions
root = customtkinter.CTk()
frm = master=root
root.title("")

Info = subprocess.run("bluetoothctl info 2C:E0:32:A5:D4:26", shell = True, stdout = subprocess.PIPE)

def Close():
    root.destroy()
def InfoCom():
    customtkinter.CTkLabel(frm, text=Info.stdout.decode(), padding=10,borderwidth=3, relief="sunken").pack()
def ConnectCom():
    os.system("bluetoothctl connect 2C:E0:32:A5:D4:26")
    customtkinter.CTkLabel(frm, text=f"Connected {DeviceName}", padding=10,borderwidth=3, relief="sunken").pack()
def DisconnectCom():
    os.system("bluetoothctl disconnect 2C:E0:32:A5:D4:26")
    customtkinter.CTkLabel(frm, text=f"Disconnected {DeviceName}", padding=10,borderwidth=3, relief="sunken").pack()

#making objects
customtkinter.CTkLabel(frm, text="Please choose an option").pack()
customtkinter.CTkLabel(frm, text=f"for your {DeviceName}").pack()
customtkinter.CTkLabel(frm, text=f"With the MAC Address: {DeviceMacAdd}").pack()

customtkinter.CTkButton(frm, text="Info", command=InfoCom).pack()
customtkinter.CTkButton(frm, text="Connect", command=ConnectCom).pack()
customtkinter.CTkButton(frm, text="Disconnect", command=DisconnectCom).pack()
customtkinter.CTkButton(frm, text="Launch nukes against Elli", command=Close).pack()

#Automatically close the window after 10 seconds
root.after(10000,lambda:Close())

#displaying objects
root.mainloop()