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

SPEED_MAX = 2047
SPEED_STEP = 128

def getSpeed(voltage,motor_idx):

    message = bytes([CMD_MOTOR_SPEED_READ])
    message+= bytes([motor_idx])
    prcControl.txMessage(message)
    
    isMessage=False
    while not isMessage:
        prcControl.rxTask()
        message = prcControl.rxMessage()
        if len(message) != 0:
            if message[0] == CMD_MOTOR_SPEED_READ_R:
                speed=struct.unpack('h',message[1:3])
                isMessage=True

    return speed

def scanSpeedVsVoltage(motor, motorIdx, minVoltage, maxVoltage, stepVoltage):
    voltage=np.array([])
    speed=np.array([])

    for curVoltage in range(minVoltage,maxVoltage,stepVoltage):
        motor.setMotor(1, 0, curVoltage)
        # PRC compute speed every second -> wait slightly more
        time.sleep(1.1)
    
        voltage=np.append(voltage, curVoltage)
        speed=np.append(speed, getSpeed(voltage,motorIdx))

    motor.setMotor(0, 0, 0)

    return (voltage, speed)

def usage():
    print("Usage: %s [options]" % sys.argv[0])
    print(" -c <port> : serial port (default COM4:)")
    print(" -w <idx>  : wheel idx (default 0)")
    print(" -s <step> : speed step (default 128)")

if __name__ == '__main__':

    # Check Arguments
    try:
        opts, args = getopt.getopt(sys.argv[1:], 'hw:s:c:')
    except getopt.GetoptError as err:
        print(err)
        sys.exit(2)
        
    comPort='COM4:'
    motorIdx=0

    try:
        for o, a in opts:
            if o == "-c":
                comPort = a
            elif o == "-w":
                motorIdx = int(a)
            elif o == "-s":
                SPEED_STEP = int(a)
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


    # Disable all logs
    message = bytes([CMD_LOG_RT, 0])
    prcControl.txMessage(message)

    # Flush Rx Buffer
    prcControl.rxTask()
    prcControl.clearRxBuffer()

    # Collect Data (run from 0 to max/min speed to avoid asymmety)
    motor = cMotor(prcControl,logger)
    (Vpos, SpeedPos) = scanSpeedVsVoltage(motor, motorIdx, 0, SPEED_MAX, SPEED_STEP)
    # Wait 2s for the motor to effectively stop
    time.sleep(2)
    (Vneg, SpeedNeg) = scanSpeedVsVoltage(motor, motorIdx, 0, -SPEED_MAX, -SPEED_STEP)
    V     = np.append(Vneg[::-1], Vpos)
    Speed = np.append(SpeedNeg[::-1], SpeedPos)

    # Initialize Pyplot 
    fig=plt.figure()
    ax=fig.add_subplot(111)
    ax.plot(V, Speed, 'r') 

    # Linear Regressions (on first/last quarter)
    segLen=len(V)//4
    x=np.array([V[0:segLen],np.ones(segLen)])
    y=Speed[0:segLen]
    resNeg = np.linalg.lstsq(x.T,y)[0]
    ax.plot(x[0],resNeg[0]*x[0]+resNeg[1],'b.')

    x=np.array([V[len(V)-segLen:],np.ones(segLen)])
    y=Speed[len(V)-segLen:]
    resPos = np.linalg.lstsq(x.T,y)[0]
    ax.plot(x[0],resPos[0]*x[0]+resPos[1],'b.')

    # Grid and labels
    ax.grid()
    plt.title('Motor speed vs code [GM = %.2f (neg), %.2f (pos)]' % (resNeg[0], resPos[0]))
    plt.xlabel('Digital code (lsb)')
    plt.ylabel('Speed (deg/s)')
    plt.show()

    now=datetime.datetime.now()
    motorName=['left', 'right']
    filename = 'logs\%02d%02d%02d_%02d%02d%02d_motorSpeed_%s.log' % (now.year, now.month, now.day, now.hour, now.minute, now.second, motorName[motorIdx])
    hFile=open(filename, 'w')
    hFile.write('# Wheel idx   : %d\n' % motorIdx)
    hFile.write('# Speed steps : %d\n' % SPEED_STEP)
    hFile.write('# Linear regression results\n')
    hFile.write('#  - Neg %.3f*V%+.3f\n' % (resNeg[0],resNeg[1]))
    hFile.write('#  - Pos %.3f*V%+.3f\n' % (resPos[0],resPos[1]))
    for i in range(len(V)):
        hFile.write('%.3f %.3f\n' % (V[i], Speed[i]))
    hFile.close()
    print("Measurement saved in %s\n" % filename)
    
