from commands import *

# ------------------------------------------------------------------------------
# -- CLASS: cLogger ------------------------------------------------------------
# ------------------------------------------------------------------------------
class cLogger:

    def __init__(self, prcControl):
        self.prcControl = prcControl
        self.logmask = 0

    def setLog(self):
        message = bytes([CMD_LOG_RT, self.logmask])
        self.prcControl.txMessage(message)

    def enable(self, mask):
        self.logmask |= mask
        self.setLog()

    def disable(self, mask):
        self.logmask &= ~mask
        self.setLog()
