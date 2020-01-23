import struct
import tkinter as tk
from commands import *

# ------------------------------------------------------------------------------
# -- CLASS: cDebug -------------------------------------------------------------
# ------------------------------------------------------------------------------
class cDebug:
    
    def __init__(self, prcControl, rootWindow, x, y, width, height):
        self.rootWindow = rootWindow
        self.prcControl = prcControl
        self.status     = tk.IntVar(value=0)
        self.data       = tk.IntVar(value=0)
        self.logValue   = tk.IntVar(value=0)
        
        self.createTkFrame(x, y, width, height)

    def createTkFrame(self, x, y, width, height):
        # Debug Frame
        mainFrameLabel=tk.Label(self.rootWindow, text="Debug", font=('Helvetica',12,'bold'))
        mainFrameLabel.grid(row=x,column=y)
        mainFrame = tk.Frame(self.rootWindow, borderwidth=3, relief=tk.GROOVE, padx=10, pady=10)
        mainFrame.grid(row=x+1,column=y, sticky=tk.NW)

        # Debug Controls 
        LogButton=tk.Button(mainFrame, text="Log", width=10, command=self.setLog)
        LogButton.grid(row=0,column=0, sticky=tk.W)

        LogValueEntry = tk.Entry(mainFrame, textvariable=self.logValue)
        LogValueEntry.grid(row=0,column=1, sticky=tk.W)

        debugButton=tk.Button(mainFrame, text="Debug", width=10, command=self.debug)
        debugButton.grid(row=1,column=0, sticky=tk.W)

        debugEntry = tk.Entry(mainFrame, textvariable=self.data)
        debugEntry.grid(row=1,column=1, sticky=tk.W)
        
        statusLabel = tk.Label(mainFrame, text="Status:")
        statusLabel.grid(row=2, column=0, sticky=tk.W)

        statusValue=tk.Label(mainFrame, textvariable=self.status, font=('Helvetica',12,'bold'))
        statusValue.grid(row=2,column=1, sticky=tk.W)

        placeholderH = tk.Frame(mainFrame, width=width, height=1)
        placeholderH.grid(row=3, column=0, columnspan=2)

        placeholderV = tk.Frame(mainFrame, width=1, height=height)
        placeholderV.grid(row=0, column=2, rowspan=4)

    def setLog(self):
        message = bytes([CMD_LOG_RT])
        message += bytes([self.logValue.get()])
        self.prcControl.txMessage(message)

    def debug(self):
        message = bytes([CMD_DEBUG])
        message+=struct.pack('h',self.data.get())

        self.prcControl.txMessage(message)

    def rxCallback(self, command):
        try:
            if command[0] == CMD_DEBUG_R:
                self.status.set(struct.unpack('i',command[1:5])[0])
        except:
            print('DEBUG: unexpected command <- %s' % command)
        

