import struct
import tkinter as tk
from commands import *

# ------------------------------------------------------------------------------
# -- CLASS: cCodeBarre -------------------------------------------------------------
# ------------------------------------------------------------------------------
class cCodeBarre:

    def __init__(self, prcControl, logger, rootWindow, x, y, width, height):
        self.rootWindow = rootWindow
        self.prcControl = prcControl
        self.logger     = logger
        self.logEnable  = tk.IntVar(value=0)
        self.code       = tk.IntVar(value=0)
        self.scanType   = tk.IntVar(value=1)
        self.status     = tk.IntVar(value=0)

        self.createTkFrame(x, y, width, height)

    def createTkFrame(self, x, y, width, height):
        # Encoder Frame
        mainFrameLabel=tk.Label(self.rootWindow, text="CodeBarre", font=('Helvetica',12,'bold'))
        mainFrameLabel.grid(row=x,column=y)
        mainFrame = tk.Frame(self.rootWindow, borderwidth=3, relief=tk.GROOVE, padx=10, pady=10)
        mainFrame.grid(row=x+1,column=y, sticky=tk.NW)

        # Code bar controls 
        logTickBox=tk.Checkbutton(mainFrame, text="Enable log", variable=self.logEnable, command=self.setLog)
        logTickBox.grid(row=0, column=0, sticky=tk.W)

        singleButton=tk.Radiobutton(mainFrame, text="Single", variable=self.scanType, value=1)
        singleButton.grid(row=1, column=0, stick=tk.W)

        doubleButton=tk.Radiobutton(mainFrame, text="Double", variable=self.scanType, value=2)
        doubleButton.grid(row=1, column=1, stick=tk.W)

        scanButton=tk.Button(mainFrame, text="Scan", width=10, command=self.scan)
        scanButton.grid(row=2,column=0, sticky=tk.W)

        readButton=tk.Button(mainFrame, text="Read", width=10, command=self.readCode)
        readButton.grid(row=3,column=0, sticky=tk.W)

        codeLabel=tk.Label(mainFrame, textvariable=self.code, font=('Helvetica',12,'bold'))
        codeLabel.grid(row=3,column=1, sticky=tk.W)
        
        statusLabel = tk.Label(mainFrame, text="Status:")
        statusLabel.grid(row=4, column=0, sticky=tk.W)

        statusValue=tk.Label(mainFrame, textvariable=self.status, font=('Helvetica',12,'bold'))
        statusValue.grid(row=4,column=1, sticky=tk.W)

        placeholderH = tk.Frame(mainFrame, width=width, height=1)
        placeholderH.grid(row=5, column=0, columnspan=2)

        placeholderV = tk.Frame(mainFrame, width=1, height=height)
        placeholderV.grid(row=0, column=2, rowspan=6)

    def setLog(self):
        mask = RTLOG_FAST_BARCODE_MASK
        if self.logEnable.get()==1:
            self.logger.enable(mask)
        else:
            self.logger.disable(mask)

    def readCode(self):
        message = bytes([CMD_CBARRE_READ])
        self.prcControl.txMessage(message)

    def scan(self):
        message = bytes([CMD_CBARRE_SCAN])
        message += bytes([self.scanType.get()])
        self.prcControl.txMessage(message)

    def rxCallback(self, command):
        try:
            if command[0] == CMD_LOG_RT_R:
                if command[1] == RTLOG_FAST_BARCODE:
                    logValue        = struct.unpack('l',command[2:6])[0]
                    level           =  logValue        & 0x1
                    state           = (logValue >> 1)  & 0x7
                    interval        = (logValue >> 4)  & 0xff
                    counter         = (logValue >> 12) & 0xf
                    samplePoint     = (logValue >> 16) & 0xff
                    currentPosition = (logValue >> 24) & 0xff

                    self.status.set(state)
                    #print('[bar] level=%1d state=%2d interval=0x%02x counter=%2d samplePoint=%02x currentPosition=%02x' % (level, state, interval, counter, samplePoint, currentPosition))
            if command[0] == CMD_CBARRE_READ_R:
                self.code.set(command[1])

        except:
            print('CBAR: unexpected command <- %s' % command)

