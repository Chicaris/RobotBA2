import struct
import tkinter as tk
from commands import *

# ------------------------------------------------------------------------------
# -- CLASS: cLed ---------------------------------------------------------------
# ------------------------------------------------------------------------------
class cLed:

    def __init__(self, rootWindow, x, y, width, height):
        self.rootWindow = rootWindow
        self.value      = tk.IntVar(value=0)

        self.createTkFrame(x, y, width, height)

    def createTkFrame(self, x, y, width, height):
        # Led Frame
        mainFrameLabel=tk.Label(self.rootWindow, text="LEDs", font=('Helvetica',12,'bold'))
        mainFrameLabel.grid(row=x,column=y)
        mainFrame=tk.Frame(self.rootWindow, borderwidth=3, relief=tk.GROOVE, padx=10, pady=10)
        mainFrame.grid(row=x+1,column=y, sticky=tk.NW)

        # LED controls
        ledValueEntry = tk.Entry(mainFrame, textvariable=self.value)
        ledValueEntry.grid(row=0,column=0, sticky=tk.W)

        ledSetValueButton=tk.Button(mainFrame, text="Set", width=10, command=self.setLed)
        ledSetValueButton.grid(row=0,column=1, sticky=tk.W)

        placeholderH = tk.Frame(mainFrame, width=width, height=1)
        placeholderH.grid(row=1, column=0, columnspan=2)

        placeholderV = tk.Frame(mainFrame, width=1, height=height)
        placeholderV.grid(row=0, column=2, rowspan=3)



