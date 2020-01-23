import struct
import tkinter as tk
import numpy as np
import matplotlib.figure as figure
import matplotlib.backends.backend_tkagg as tkagg

from commands import *

# ------------------------------------------------------------------------------
# -- CLASS: cOdometer ----------------------------------------------------------
# ------------------------------------------------------------------------------
class cOdometer:

    def __init__(self, rootWindow, x, y, width, height):
        self.rootWindow = rootWindow

        self.logEnable = tk.IntVar(value=0)
        self.xpos      = tk.StringVar(value=0)
        self.ypos      = tk.StringVar(value=0)
        self.angle     = tk.StringVar(value=0)

        self.x         = np.array([])
        self.y         = np.array([])
        self.a         = np.array([])

        self.createTkFrame(x, y, width, height)

    def createTkFrame(self, x, y, width, height):
        # Odometer Frame
        mainFrameLabel=tk.Label(self.rootWindow, text="Odometer", font=('Helvetica',12,'bold'))
        mainFrameLabel.grid(row=x,column=y, columnspan=2)
        mainFrame = tk.Frame(self.rootWindow, borderwidth=3, relief=tk.GROOVE, padx=10, pady=10)
        mainFrame.grid(row=x+1,column=y, columnspan=2, rowspan=5, sticky=tk.NW)

        # Real time log
        odometerEnable=tk.Checkbutton(mainFrame, text="Enable log", variable=self.logEnable, command=self.setLog)
        odometerEnable.grid(row=0, column=0, sticky=tk.W)

        # Mission Controls 
        resetButton=tk.Button(mainFrame, text="Reset position", width=20, command=self.resetPosition)
        resetButton.grid(row=0,column=1, sticky=tk.W)

        
        # Odometer Controls
        odometerXLabel = tk.Label(mainFrame, text="X(cm)")
        odometerXLabel.grid(row=1, column=0, sticky=tk.W)

        odometerXLabel = tk.Label(mainFrame, textvariable=self.xpos)
        odometerXLabel.grid(row=1,column=1, sticky=tk.W)

        odometerYLabel = tk.Label(mainFrame, text="Y(cm)")
        odometerYLabel.grid(row=1, column=2, sticky=tk.W)

        odometerYLabel = tk.Label(mainFrame, textvariable=self.ypos)
        odometerYLabel.grid(row=1,column=3, sticky=tk.W)

        odometerALabel = tk.Label(mainFrame, text="Angle(deg)")
        odometerALabel.grid(row=1, column=4, sticky=tk.W)

        odometerAEntry = tk.Label(mainFrame, textvariable=self.angle)
        odometerAEntry.grid(row=1,column=5, sticky=tk.W)

        # Matplotlib plot
        bckgColor = mainFrame.winfo_rgb(mainFrame['bg'])
        plotarea = figure.Figure(figsize=(4,4), dpi=100, facecolor=(bckgColor[0]/65535,bckgColor[1]/65535,bckgColor[2]/65535))
        subplot= plotarea.add_subplot(111)
        self.line=subplot.plot([],[],'r-')[0]
        self.axes=plotarea.axes[0]
        self.axes.grid()

        self.canvas=tkagg.FigureCanvasTkAgg(plotarea, master=mainFrame)
        self.canvas.get_tk_widget().grid(row=2, column=0, columnspan=6, sticky=tk.N)
        self.canvas.show()
        
        # Place holders
        placeholderH = tk.Frame(mainFrame, width=width, height=1)
        placeholderH.grid(row=3, column=0, columnspan=6)

        placeholderV = tk.Frame(mainFrame, width=1, height=height)
        placeholderV.grid(row=0, column=6, rowspan=4)

    def resetPosition(self):
        self.x         = np.array([])
        self.y         = np.array([])
        self.a         = np.array([])


