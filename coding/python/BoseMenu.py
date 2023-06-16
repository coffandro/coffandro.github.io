#importing libraries
from distutils.log import info
import os
import subprocess
from tkinter import *
from tkinter import ttk
import importlib

#setup of variables
DeviceName="Bose Quietcomfort 35 II"
DeviceMacAdd="4C:87:5D:0D:78:09"

#setup of functions
root = Tk()
frm = ttk.Frame(root, padding=1)
frm.grid()
root.title("")

Info = subprocess.run("bluetoothctl info 4C:87:5D:0D:78:09", shell = True, stdout = subprocess.PIPE)

def Close():
    root.destroy()
def InfoCom():
    ttk.Label(frm, text=Info.stdout.decode(), padding=10,borderwidth=3, relief="sunken").pack()
def ConnectCom():
    os.system("bluetoothctl connect 4C:87:5D:0D:78:09")
    ttk.Label(frm, text=f"Connected {DeviceName}", padding=10,borderwidth=3, relief="sunken").pack()
def DisconnectCom():
    os.system("bluetoothctl disconnect 4C:87:5D:0D:78:09")
    ttk.Label(frm, text=f"Disconnected {DeviceName}", padding=10,borderwidth=3, relief="sunken").pack()

#making objects
ttk.Label(frm, text="Please choose an option").pack()
ttk.Label(frm, text=f"for your {DeviceName}").pack()
ttk.Label(frm, text=f"With the MAC Address: {DeviceMacAdd}").pack()

ttk.Button(frm, text="Info", command=InfoCom).pack()
ttk.Button(frm, text="Connect", command=ConnectCom).pack()
ttk.Button(frm, text="Disconnect", command=DisconnectCom).pack()
ttk.Button(frm, text="Launch nukes against Elli", command=Close).pack()

#Automatically close the window after 10 seconds
root.after(10000,lambda:Close())

#displaying objects
root.mainloop()