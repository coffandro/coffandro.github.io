#Import the tkinter library
from tkinter import *


#Create an instance of tkinter frame
win = Tk()

#Create a Label to print the Name
label= Label(win, text="This is a New Line Text", font= ('Helvetica 14 bold'), foreground= "red3")
label.pack()

win.wm_attributes('-fullscreen', 'True')
win.mainloop()