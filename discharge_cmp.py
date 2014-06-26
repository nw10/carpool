import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure(figsize=(6,8))

censor = np.loadtxt('censor_rec.txt')

hov_up = np.loadtxt('../carpool/data/0314/770407_count.txt')
hov_down = np.loadtxt('../carpool/data/0314/768945_count.txt')

gp_up_total = np.loadtxt('../carpool/data/0314/765477_count.txt') + \
              np.loadtxt('../carpool/data/0314/717679_count.txt') + \
              np.loadtxt('../carpool/data/0314/717683_count.txt') 

gp_down_total = np.loadtxt('../carpool/data/0314/765486_count.txt') + \
                np.loadtxt('../carpool/data/0314/761228_count.txt')


qb = 1300
plt.subplot(211)

indices = (censor[:,1] == 200) & (censor[:,0] == 0)
plt.plot(censor[indices,2], censor[indices,3] - censor[indices,2]*qb/3600.0, 'g--')
plt.plot(np.linspace(0,180*30,180), hov_up[75:255].cumsum() - hov_up[75] - np.linspace(0,180*30,180)*qb/3600.0, 'g')

indices = (censor[:,1] == 1600) & (censor[:,0] == 0)
plt.plot(np.linspace(0,180*30,180), hov_down[75:255].cumsum() - hov_down[75] -  np.linspace(0,180*30,180)*qb/3600.0, 'r')
plt.plot(censor[indices,2][50:], censor[indices,3][50:] - (censor[indices,2][50:] - censor[indices,2][50])*qb/3600.0, 'r--')



qb = 7200
plt.subplot(212)

indices = (censor[:,1] == 200) & (censor[:,0] > 0)
time_series = censor[indices, 2][::5]
cout_series = censor[indices, 3][::5] + censor[indices, 3][1::5] + censor[indices, 3][2::5] + \
              censor[indices, 3][3::5] + censor[indices, 3][4::5]
plt.plot(np.linspace(0,180*30,180), gp_up_total[75:255].cumsum() - gp_up_total[75] - np.linspace(0,180*30,180)*qb/3600.0, 'g')
plt.plot(time_series, cout_series - time_series*qb/3600.0, 'g--')


indices = (censor[:,1] == 1600) & (censor[:,0] > 0)
time_series = censor[indices, 2][::5]
cout_series = censor[indices, 3][::5] + censor[indices, 3][1::5] + censor[indices, 3][2::5] + \
              censor[indices, 3][3::5] + censor[indices, 3][4::5]
plt.plot(np.linspace(0,180*30,180), gp_down_total[75:255].cumsum() - gp_down_total[75] - np.linspace(0,180*30,180)*qb/3600.0, 'r')
plt.plot(time_series[50:], cout_series[50:] - (time_series[50:]-time_series[50])*qb/3600.0, 'r--')



plt.show()





