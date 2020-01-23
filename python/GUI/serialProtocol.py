import tkinter as tk
from prcRadio import PrcRadio

# ------------------------------------------------------------------------------
# -- CLASS: Communication ------------------------------------------------------
# ------------------------------------------------------------------------------

class cSerialProtocol(PrcRadio):

    CMD_EOM = 0xa5
    CMD_ESC = 0x1b
    escapeDict = {
        CMD_EOM : 0x01,
        CMD_ESC : 0x02
    }

    def __init__(self, portName, rootWindow=None, x=0, y=0, width=100, height=100, debug=False):

        PrcRadio.__init__(self,portName)
        self.rootWindow  = rootWindow
        self.rxBuffer    = bytes()
        self.debug       = debug

        if rootWindow != None:
            self.baseTitle   = rootWindow.title()
            self.serPortName = tk.StringVar(value=portName)
            self.createTkFrame(x, y, width, height)
        else:
            self.serPortName = portName

    def __del__(self):
        self.disconnect()

    def createTkFrame(self, x, y, width, height):
        # Serial Frame
        mainFrameLabel=tk.Label(self.rootWindow, text="Serial Settings", font=('Helvetica',12,'bold'))
        mainFrameLabel.grid(row=x,column=y)
        mainFrame = tk.Frame(self.rootWindow, borderwidth=3, relief=tk.GROOVE, padx=10, pady=10)
        mainFrame.grid(row=x+1,column=y, sticky=tk.NW)

        # Serial Control
        serialPortLabel = tk.Label(mainFrame, text="Serial port")
        serialPortLabel.grid(row=0,column=0, sticky=tk.W)

        serialPortEntry = tk.Entry(mainFrame, textvariable=self.serPortName)
        serialPortEntry.grid(row=0,column=1, sticky=tk.W)

        serialMoveButton=tk.Button(mainFrame, text="Connect", width=10, command=self.connect)
        serialMoveButton.grid(row=0,column=2, sticky=tk.W)

        placeholderH = tk.Frame(mainFrame, width=width, height=1)
        placeholderH.grid(row=1, column=0, columnspan=3)

        placeholderV = tk.Frame(mainFrame, width=1, height=height)
        placeholderV.grid(row=0, column=3, rowspan=2)

    def connect(self):
        if self.isConnected:
            self.disconnect()

        if self.rootWindow != None:
            self.serialPort.port = self.serPortName.get()
        else:
            self.serialPort.port = self.serPortName
            
        if not(PrcRadio.connect(self)):
            print('Echec de la connexion')

        if self.rootWindow != None:
            if self.isConnected():
                self.rootWindow.title(self.baseTitle + ' (Connected)')
            else:
                self.rootWindow.title(self.baseTitle + ' (DISCONNECTED)')

    def clearRxBuffer(self):
        self.rxBuffer= bytes()
        
    def txMessage(self, message):
        dataBytes = self.messageEncode(message)
        if self.debug:
            self.printByteString('tx:', dataBytes)
        try:
            self.serialPort.write(dataBytes)
            self.success = True
        except:
            self.success = False
        return self.success

    def rxMessage(self):
        dataBytes=bytes()
        # Check for a complete message in the rx queue
        msgLength = self.rxBuffer.find(self.CMD_EOM)
        if msgLength != -1:
            # Extract data and remove trailing CDM_EOM
            dataBytes=self.messageDecode(self.rxBuffer[0:msgLength])
            if self.debug:
                self.printByteString('rx:', dataBytes)
            # Remove message from the buffer
            self.rxBuffer=self.rxBuffer[msgLength+1:]

        return dataBytes
            
    def messageEncode(self, data):
        outputMessage=bytes()
        for byte in data:
            if byte in self.escapeDict:
                outputMessage+=bytes([self.CMD_ESC, self.escapeDict[byte]])
            else:
                outputMessage+=bytes([byte])
        outputMessage+=bytes([self.CMD_EOM])
        
        return outputMessage

    def messageDecode(self, data):
        outputMessage=bytes()
        isEscape=False
        for byte in data:
            if isEscape:
                outputMessage+=bytes([list(self.escapeDict.keys())[list(self.escapeDict.values()).index(byte)]])
                isEscape=False
            elif byte == self.CMD_ESC:
                isEscape=True
            else:
                outputMessage+=bytes([byte])
       
        return outputMessage

    # Should be called on regular basis to fetch data from the serial interface
    def rxTask(self):
        nBytes=self.serialPort.inWaiting()
        try:
            self.rxBuffer+=self.serialPort.read(nBytes)
            self.success = True
        except:
            self.success = False

        return self.success


# - Debug code -----------------------------------------------------------------

    def printByteString(self, prefix, message):
        lineOut=prefix
        for byte in message:
            lineOut+='0x%02x ' % byte
        print(lineOut)


