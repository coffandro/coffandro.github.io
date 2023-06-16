# Import tkinter and webview libraries
import tkinter as tk
from tkinter import ttk
  
# define an instance of tkinter
root = tk.Tk()
root.geometry('400x300')
root.title('Notebook Demo')

# create a notebook
notebook = ttk.Notebook(root)
notebook.pack(pady=10, expand=True)

# create frames
frame1 = ttk.Frame(notebook, width=400, height=280)
frame2 = ttk.Frame(notebook, width=400, height=280)

frame1.pack(fill='both', expand=True)
frame2.pack(fill='both', expand=True)

# add frames to notebook

notebook.add(frame1, text='General Information')
notebook.add(frame2, text='Social Demokratiet')
#  size of the window where we show our website
# root.geometry("800x450")
  
# Open website

root.mainloop()