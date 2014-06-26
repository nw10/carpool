import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('log.txt')

indices = data[:,1] == 0
id_list = np.unique(data[indices,2])
for id in id_list:
    ind = data[indices,2] == id
    plt.plot(data[indices,0][ind], data[indices, -2][ind], 'r')

indices = data[:,1] == 1
id_list = np.unique(data[indices,2])
for id in id_list:
    ind = data[indices,2] == id
    plt.plot(data[indices,0][ind], data[indices, -2][ind], 'g--')



plt.show()
