import tkinter as tk # Python 3
root = tk.Tk()
label = tk.Label(root, image=root.image, bg='white')
root.configure(bg='')
root.geometry("+250+250")
root.lift()
label.pack()
label.mainloop()