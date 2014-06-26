import numpy as np
import matplotlib.pylab as plt

lane = 1
data = np.loadtxt('log.txt')
data = data[data[:,1] == lane]

x_range = [np.min(data[:,0]), np.max(data[:,0])]
y_range = [np.min(data[:,3]), np.max(data[:,3])]

x = np.linspace(x_range[0], x_range[1], 100)
y = np.linspace(y_range[0], y_range[1], 100)
x_interval = x[1] - x[0]
y_interval = y[1] - y[0]

z = np.zeros((100, 100))
z_c = np.zeros((100,100))

for i in data:
    index_x = int((i[0] - x_range[0]) / x_interval)
    index_y = int((i[3] - y_range[0]) / y_interval)
    z[index_x, index_y] += i[-1]
    z_c[index_x, index_y] += 1.0

index = z_c != 0
z[index] = z[index]/z_c[index]

plt.contourf(x, y, z.T, cmap=plt.cm.jet_r)
plt.colorbar()
plt.show()
