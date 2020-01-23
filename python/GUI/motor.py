import struct
import tkinter as tk
from commands import *

# ------------------------------------------------------------------------------
# -- CLASS: cMotor -------------------------------------------------------------
# ------------------------------------------------------------------------------
class cMotor:

    def __init__(self, prcControl, logger, rootWindow=None, x=0, y=0, width=100, height=100):
        self.rootWindow = rootWindow
        self.prcControl = prcControl
        self.logger     = logger
        self.isEnabled  = False
        if rootWindow != None:
            self.x          = tk.DoubleVar(value=0.0)            
            self.y          = tk.DoubleVar(value=0.0)            
            self.distance   = tk.DoubleVar(value=0.0)
            self.angle      = tk.DoubleVar(value=0.0)
            self.logEnable  = tk.IntVar(value=0)
            self.encoder_left  = tk.IntVar(value=0)
            self.encoder_right = tk.IntVar(value=0)
            self.createTkFrame(x, y, width, height)

    def createTkFrame(self, x, y, width, height):
        # Motor Frame
        mainFrameLabel=tk.Label(self.rootWindow, text="Motors", font=('Helvetica',12,'bold'))
        mainFrameLabel.grid(row=x,column=y)
        mainFrame = tk.Frame(self.rootWindow, borderwidth=3, relief=tk.GROOVE, padx=10, pady=10)
        mainFrame.grid(row=x+1,column=y, sticky=tk.NW)
        
        # Motor Controls
        logTickBox=tk.Checkbutton(mainFrame, text="Enable log", variable=self.logEnable, command=self.setLog)
        logTickBox.grid(row=0, column=0, sticky=tk.W)

        gotoButton=tk.Button(mainFrame, text="Goto", width=10, command=self.goto)
        gotoButton.grid(row=1,column=0, sticky=tk.W)

        xCoordinateEntry = tk.Entry(mainFrame, width=10, textvariable=self.x)
        xCoordinateEntry.grid(row=1,column=1, sticky=tk.W)
        
        yCoordinateEntry = tk.Entry(mainFrame, width=10, textvariable=self.y)
        yCoordinateEntry.grid(row=1,column=2, sticky=tk.W)
        
        moveButton=tk.Button(mainFrame, text="Move", width=10, command=self.move)
        moveButton.grid(row=2,column=0, sticky=tk.W)

        moveButtonEntry = tk.Entry(mainFrame, width=10, textvariable=self.distance)
        moveButtonEntry.grid(row=2,column=1, sticky=tk.W)
        
        rotateButton=tk.Button(mainFrame, text="Rotate", width=10, command=self.rotate)
        rotateButton.grid(row=3,column=0, sticky=tk.W)

        rotateButtonEntry = tk.Entry(mainFrame, width=10, textvariable=self.angle)
        rotateButtonEntry.grid(row=3,column=1, sticky=tk.W)
        
        EncoderLabel = tk.Label(mainFrame, text="Encoders")
        EncoderLabel.grid(row=4, column=0, sticky=tk.W)

        EncoderLeftLabel=tk.Label(mainFrame, textvariable=self.encoder_left, font=('Helvetica',12,'bold'))
        EncoderLeftLabel.grid(row=4,column=1, sticky=tk.W)

        EncoderRightLabel=tk.Label(mainFrame, textvariable=self.encoder_right, font=('Helvetica',12,'bold'))
        EncoderRightLabel.grid(row=4,column=2, sticky=tk.W)

        placeholderH = tk.Frame(mainFrame, width=width, height=1)
        placeholderH.grid(row=5, column=0, columnspan=3)

        placeholderV = tk.Frame(mainFrame, width=1, height=height)
        placeholderV.grid(row=0, column=3, rowspan=6)

    def goto(self):
        message = bytes([CMD_GOTO])
        message += struct.pack('f',self.x.get())
        message += struct.pack('f',self.y.get())
        self.prcControl.txMessage(message)

    def move(self):
        message = bytes([CMD_MOVE])
        message += struct.pack('f',self.distance.get())
        self.prcControl.txMessage(message)

    def rotate(self):
        message = bytes([CMD_ROTATE])
        message += struct.pack('f',self.angle.get())
        self.prcControl.txMessage(message)

    def setLog(self):
        mask = RTLOG_ENCODER_0_MASK | RTLOG_ENCODER_1_MASK
        if self.logEnable.get()==1:
            self.logger.enable(mask)
        else:
            self.logger.disable(mask)

    def setMotor(self, enable, moveType, voltage):
        message = bytes([CMD_MOTOR_SET, enable, moveType])
        message += struct.pack('h',voltage)
        self.prcControl.txMessage(message)

    def rxCallback(self, command):
        try :
            if command[0] == CMD_LOG_RT_R:
                if command[1] == RTLOG_ENCODER_0:
                    self.encoder_left.set(struct.unpack('l',command[2:6]))
                elif command[1] == RTLOG_ENCODER_1:
                    self.encoder_right.set(struct.unpack('l',command[2:6]))
        except:
            print('MOTOR: unexpected command <- %s' % command)

