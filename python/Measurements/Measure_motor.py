import sys
import os
import time
import struct
import datetime
import getopt
import numpy as np
import matplotlib.pyplot as plt

sys.path.append(os.path.abspath('..\GUI'))

from motor import cMotor
from logger import cLogger
from serialProtocol import cSerialProtocol
from commands import *

def impulseResponse(motor, EncoderIdx, testSpeed):
    startTime = time.time()
    position=np.array([])

    # Set motors at test speed
    motor.setMotor(1, 0, testSpeed)

    # Gather data for 1s
    while (time.time() - startTime) < 1:
        isMessage=False
        while not isMessage:
            prcControl.rxTask()
            message = prcControl.rxMessage()
            if len(message) != 0:
                if message[0] == CMD_LOG_RT_R and message[1] == EncoderIdx:
                    curPosition=struct.unpack('i',message[2:6])
                    isMessage=True

        position=np.append(position, curPosition)

    # Disable Motors
    motor.setMotor(0, 0, 0)

    return position

def usage():
    print("Usage: %s [options]" % sys.argv[0])
    print(" -c <port> : serial port (default COM4:)")
    print(" -w <idx>  : wheel idx (default 0)")
    print(" -s <step> : test speed (default 1536)")

if __name__ == '__main__':

    # Check Arguments
    try:
        opts, args = getopt.getopt(sys.argv[1:], 'hw:s:c:')
    except getopt.GetoptError as err:
        print(err)
        sys.exit(2)
        
    comPort='COM4:'
    EncoderIdx=0
    testSpeed=1536

    try:
        for o, a in opts:
            if o == "-c":
                comPort = a
            elif o == "-w":
                EncoderIdx = int(a)
            elif o == "-s":
                testSpeed = int(a)
            elif o == "-h":
                usage()
                sys.exit()
            else:
                assert False, "Unhandled option: %s" % o
    except:
        print ('Syntax Error\n')
        usage()
        sys.exit(2)
            
    # Open Control Interface
    prcControl = cSerialProtocol(comPort, debug=False)
    prcControl.connect()
    # Logger
    logger = cLogger(prcControl)

    if not prcControl.success:
        print('Unable to open %s\n' % comPort)
        sys.exit(2)     

    motor = cMotor(prcControl,logger)

    # Disable all logs
    message = bytes([CMD_LOG_RT, 0])
    prcControl.txMessage(message)

    # Flush Rx Buffer
    prcControl.rxTask()
    prcControl.clearRxBuffer()

    # Enable Encoder Log
    if EncoderIdx == 0:
        message = bytes([CMD_LOG_RT, RTLOG_FAST_ENCODER_0_MASK])
    else:
        message = bytes([CMD_LOG_RT, RTLOG_FAST_ENCODER_1_MASK])
    prcControl.txMessage(message)

    # Collect Data (one sample / 10ms)
    position = impulseResponse(motor, EncoderIdx, testSpeed)
    speed=100*np.diff(position)
    t=np.arange(len(speed))*10**-2

    # Average of 5 points
    filt_mean=np.ones(5)/5
    speed_filtered= np.convolve(filt_mean, speed, 'same')

    # Digital all logs
    message = bytes([CMD_LOG_RT, 0])
    prcControl.txMessage(message)

    # Initialize Pyplot 
    fig=plt.figure()
    ax=fig.add_subplot(111)
    ax.plot(t,speed, 'r', t, speed_filtered, 'b') 

    # Grid and labels
    ax.grid()
    plt.title('Motor speed vs time')
    plt.xlabel('Time (s)')
    plt.ylabel('Speed (deg/s)')
    plt.show()

    now=datetime.datetime.now()
    motorName=['left', 'right']
    filename = 'logs\%02d%02d%02d_%02d%02d%02d_motorImpulse_%s.log' % (now.year, now.month, now.day, now.hour, now.minute, now.second, motorName[EncoderIdx])
    hFile=open(filename, 'w')
    hFile.write('# Wheel idx   : %d\n' % EncoderIdx)
    hFile.write('# Test speed  : %d\n' % testSpeed)
    for i in range(len(speed)):
        hFile.write('%.3f\n' % speed[i])
    hFile.close()
    print("Measurement saved in %s\n" % filename)
    
