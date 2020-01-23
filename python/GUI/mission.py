import struct
import tkinter as tk
from commands import *

# ------------------------------------------------------------------------------
# -- CLASS: cMission -------------------------------------------------------------
# ------------------------------------------------------------------------------
class cMission: 

    def __init__(self, prcControl, rootWindow, x, y, width, height):
        self.rootWindow = rootWindow
        self.prcControl = prcControl
        self.status     = tk.StringVar()
        self.logValue   = tk.IntVar(value=0)

        self.createTkFrame(x, y, width, height)

    def createTkFrame(self, x, y, width, height):
        # Mission Frame
        mainFrameLabel=tk.Label(self.rootWindow, text="Mission", font=('Helvetica',12,'bold'))
        mainFrameLabel.grid(row=x,column=y)
        mainFrame = tk.Frame(self.rootWindow, borderwidth=3, relief=tk.GROOVE, padx=10, pady=10)
        mainFrame.grid(row=x+1,column=y, sticky=tk.NW)

        # Mission Controls 
        GoButton=tk.Button(mainFrame, text="Start", width=10, command=self.startMission)
        GoButton.grid(row=0,column=0, sticky=tk.W)

        statusLabel = tk.Label(mainFrame, text="Status:")
        statusLabel.grid(row=1, column=0, sticky=tk.W)

        statusValue=tk.Label(mainFrame, textvariable=self.status, font=('Helvetica',12,'bold'))
        statusValue.grid(row=1,column=1, sticky=tk.W)

        placeholderH = tk.Frame(mainFrame, width=width, height=1)
        placeholderH.grid(row=2, column=0, columnspan=2)

        placeholderV = tk.Frame(mainFrame, width=1, height=height)
        placeholderV.grid(row=0, column=2, rowspan=3)

    def startMission(self):
        message = bytes([CMD_GO])
        self.prcControl.txMessage(message)

    def rxCallback(self, command):
        try :
            if command[0] == CMD_GO_R:
                if command[1] == 0:
                    self.status.set("Mission completed")
                if command[1] == 1:
                    self.status.set("Mission ongoing ...")
                if command[1] == 2:
                    self.status.set("Mission failed ...")
        except:
            print('Mission: unexpected command <- %s' % command)

