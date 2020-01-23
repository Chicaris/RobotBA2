import sys
import numpy as np
import matplotlib.pyplot as plt

if __name__ == "__main__":
    filename = 'data.log'
    if len(sys.argv) > 1:
        filename = sys.argv[1]

try:
    hFile = open(filename, 'r')
except:
    print('ERROR: Unable to open %s\n' % filename) 
    sys.exit(2)

x=np.array([])
y=np.array([])
for line in hFile:
    elements = line.strip().split()
    # Skip comments
    if elements[0][0] != '#':
        try:
            x=np.append(x, float(elements[0]))
            y=np.append(y, float(elements[1]))
        except:
            print('ERROR: Incorrect format : %s\n' % line)
            sys.exit(2)

hFile.close()

plt.figure(1)
plt.plot(x,y)
plt.grid()
plt.title('Data from %s' % filename)
plt.xlabel('xlabel')
plt.ylabel('ylabel')
plt.show()
