#!/usr/bin/env python3
import pandas as pd
from sys import argv

data = pd.read_csv(argv[1],sep='\s+')
data['size'] = data['size']*4
nmodes = len(data["mode"].unique())
data['idx'] = data.index//nmodes
data = data.set_index(['idx','mode','size','repeats'])
data = data.unstack('mode')
data = data.reset_index()
data.to_csv('data.csv',sep='\t')

# Plotting
from matplotlib import pyplot as plt
plt.title('Time')
for meas in data.total_time:
    #plt.plot(data['size'], data.total_time[meas], label = meas, linestyle = 'None', marker = '+')
    plt.plot(data['size'], data.total_time[meas]/data.repeats, label = meas)
   
plt.xscale('log')
plt.yscale('log')
plt.grid()
plt.legend()

plt.figure()
plt.title('BW')
for meas in data.total_time:
    #plt.plot(data['size'], data.total_time[meas], label = meas, linestyle = 'None', marker = '+')
    plt.plot(data['size'], data['size']/(data.total_time[meas]/data.repeats), label = meas)

plt.xscale('log')
plt.yscale('log')
plt.legend()


plt.show()
