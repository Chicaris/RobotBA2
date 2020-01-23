import struct
import tkinter

from serialProtocol import cSerialProtocol
from logger import cLogger
from motor import cMotor
from led   import cLed
from debug import cDebug
from mission import cMission
from odometer import cOdometer
from codebarre import cCodeBarre

from commands import *

# ------------------------------------------------------------------------------
# -- CLASS: prcControl ---------------------------------------------------------
# ------------------------------------------------------------------------------
    
class cPrcControl:
    def __init__(self, title, port):

        self.rootWindow=tkinter.Tk(className=title)

        # Control Interface
        self.prcControl=cSerialProtocol(port, self.rootWindow, 0, 0, 300, 50, debug=False)
        self.prcControl.connect()
        # Logger
        logger = cLogger(self.prcControl)

        # Define PRC objects
        self.componentList = (
            cMotor(self.prcControl,logger,     self.rootWindow, 2, 0, 300, 130),
            cMission(self.prcControl,          self.rootWindow, 4, 0, 300, 100),
            cLed(self.prcControl,              self.rootWindow, 0, 1, 300, 50),
            cCodeBarre(self.prcControl,logger, self.rootWindow, 2, 1, 300, 130),
            cDebug(self.prcControl,            self.rootWindow, 4, 1, 300, 100),
            cOdometer(self.prcControl,logger,  self.rootWindow, 0, 2, 627, 100)
        )
       
        self.createMenu()

        self.backGroundTask()

    def createMenu(self):
        # Menu
        menuBar = tkinter.Menu(self.rootWindow)

        scenarioMenu = tkinter.Menu(menuBar, tearoff=0)
        scenarioMenu.add_command(label="Scenario", command=self.notImplemented)
        menuBar.add_cascade(label="Load", menu=scenarioMenu)

        logsMenu = tkinter.Menu(menuBar, tearoff=0)
        logsMenu.add_command(label="Load", command=self.notImplemented)
        logsMenu.add_command(label="Save", command=self.notImplemented)
        menuBar.add_cascade(label="Logs", menu=logsMenu)

        menuBar.add_command(label="Exit", command=self.exitApplication)

        self.rootWindow.config(menu=menuBar)

# - GUI implementation code ----------------------------------------------------

    def notImplemented(self):
        print('Function not implemented !')
        pass

    def backGroundTask(self):
        # Fetch data from serial ifce (if connected)
        if self.prcControl.isConnected() and self.prcControl.rxTask():
            isMessage=True
            while isMessage:
                message = self.prcControl.rxMessage()
                if len(message) != 0:
                    for component in self.componentList:
                       component.rxCallback(message)
                else:
                    isMessage=False

        # reschedule in 100ms (No dedicated thread required, 115200 bps->14.4KB/s->1.5KB max. to process/typ. smaller than serial driver rx buffer)
        self.rootWindow.after(100, self.backGroundTask)

    def run(self):
        self.rootWindow.mainloop()

    def exitApplication(self):
        self.rootWindow.destroy()

# ------------------------------------------------------------------------------
# - Main code ------------------------------------------------------------------
# ------------------------------------------------------------------------------

if __name__ == '__main__':
    app=cPrcControl('PRC Robot Controller', 'COM4:')
    app.run()
