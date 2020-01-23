import numpy as np
import matplotlib.pyplot as plt

dt  = 0.01  # s (Loop update rate, 10ms)
R0  = 0.025 # m (Nominal wheel radius, 2.5cm)
W   = 0.4   # m (Nominal distance between wheels, 40cm)
dGM = +0.00  # % Gain error between left and right motors
dR  = +0.05  # % Radius error between left and right wheels

#                  +               Motor
# pos_in -->|GE|-->O-->|Kloop|--->|GM/s|---+--> pos_out
#   (m)           -^                       |      (m)
#                  |        Encoder        |
#                  +---------|GE|----------+
#                     (deg)
#
# Open loop gain = Kloop*GE*GM/(2*pi*f)
#

# Encoder gain, GE = 360/(2*pi*R), deg/m
GE0 = 360.0/(2*np.pi*R0) 
# Nominal motor gain, ex. [-2048 2048] <-> [-0.2 0.2] m/s
#GM0 = 0.2/2048
GM0 = 0.44 * np.pi/180.0 * R0

f0 = 1 # Hz (Loop bandwidth target -> Open loop gain = 1 at that frequency)
Kloop = (2*np.pi*f0)/GM0/GE0
print("Kloop = %.2f" % Kloop)

x=np.array([0.0])
y=np.array([0.0])
a=np.array([0.0])

# Left and right Motor Gain/Wheel diameter (encoder gain)
GM    ={'left':GM0*(1+dGM) , 'right':GM0}
GE    ={'left':GE0*(1+dR)  , 'right':GE0}

NSTEPS=1000
step = 0.1

# Input (increase distance by step)
Pos_in=step * GE0 * np.ones(NSTEPS)

Vmotor={'left':np.array([0]), 'right':np.array([0])}
Smotor={'left':np.array([0]), 'right':np.array([0])}
Pos_out  ={'left':np.array([0]), 'right':np.array([0])}
Dfb   ={'left':np.array([0]), 'right':np.array([0])}
dPos_out ={'left':0,  'right':0}
# Time simulation
for t in range(NSTEPS):
   for side in ['left', 'right']:
      Vmotor[side] = np.append(Vmotor[side], Kloop * (Pos_in[t]-Dfb[side][-1]))
      Smotor[side] = np.append(Smotor[side], GM[side] * Vmotor[side][-1])
      dPos_out[side]  = Pos_out[side][-1] + Smotor[side][-1] * dt
      Pos_out[side]   = np.append(Pos_out[side], dPos_out[side])
      Dfb[side]    = np.append(Dfb[side],    GE[side] * Pos_out[side][-1])

   # Odometry
   dPos_outMean = (Smotor['right'][-1] + Smotor['left'][-1])*dt/2.0
   dAout= np.arctan((Smotor['right'][-1] - Smotor['left'][-1])*dt/2.0/W)

   x = np.append(x, x[-1]+dPos_outMean * np.cos(a[-1]))
   #x = np.append(x, x[-1]+dPos_outMean * np.cos(a[-1]))
   y = np.append(y, y[-1]+dPos_outMean * np.sin(a[-1]))
   a = np.append(a, a[-1]+dAout)

plt.figure(1)
plt.title('Time evolution')
plt.ylabel('Distance')
plt.xlabel('Time (s)')
plt.grid()
t=dt*np.arange(0, NSTEPS)
distance=np.sqrt(np.power(x,2)+np.power(y,2))
plt.plot(t,distance[1:])

plt.figure(2)
plt.title('Odometer (dR=%.0f%%, dGM=%.0f%%)' % (100*dR, 100*dGM))
plt.ylabel('Y (m)')
plt.xlabel('X (m)')
plt.grid()
plt.plot(x,y)
plt.show()
