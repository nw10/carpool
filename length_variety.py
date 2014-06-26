import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

dic = {400:1600,600:1900,800:1900,1000:2000,1200:2000}
dic = {400:1600,1200:2000}

plt.figure(figsize=(7,10))
plt.subplot(211)
back = 1850
for i in dic.keys():
    data = np.loadtxt('/Users/weini/Desktop/carpool/length variaty/' + str(i) + '/censor_rec.txt')
    indices = (data[:,0] == 0) & (data[:,1] == dic[i])
    x = data[indices,-2]
    y = data[indices,-1] - data[indices,-2]*back/3600.0
    plt.plot(x, y - signal.detrend(y))

plt.subplot(212)
back = 6500
for i in dic.keys():
    data = np.loadtxt('/Users/weini/Desktop/carpool/length variaty/' + str(i) + '/censor_rec.txt')
    indices_1 = (data[:,0] == 1) & (data[:,1] == dic[i])
    indices_2 = (data[:,0] == 2) & (data[:,1] == dic[i])
    indices_3 = (data[:,0] == 3) & (data[:,1] == dic[i])
    indices_4 = (data[:,0] == 4) & (data[:,1] == dic[i])
    indices_5 = (data[:,0] == 5) & (data[:,1] == dic[i])
    cul = data[indices_1,-1] + data[indices_2,-1] + \
            data[indices_3,-1] + data[indices_4,-1] + \
            data[indices_5,-1]
    x = data[indices_1,-2]
    y = cul - x*back/3600.0
    plt.plot(x, y - signal.detrend(y))

plt.show()
