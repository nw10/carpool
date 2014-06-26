import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('log.txt')

indices = (data[:,1] == 1)

plt.plot(data[indices,0], data[indices,3],'go')
plt.show()
