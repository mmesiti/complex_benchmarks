#!/usr/bin/env python3
import pandas as pd
from sys import argv
from tabulate import tabulate

data = pd.read_csv(argv[1],sep='\s+')

complex_size = 8

data = (data
        .assign(size_MB = lambda df : df["size"].astype(float) * complex_size/ 1_000_000)
        .assign(idx = lambda df: df.index // len(df["mode"].unique()))
        .set_index(["idx","mode","size_MB","repeats"])
        .unstack("mode")
        .reset_index())

with open("data.csv", 'w') as f:
    headers = [ cname[1] if len(cname[1].strip()) != 0 else cname[0] for cname in data.columns ]
    print(headers)
    f.write(tabulate(data,
                     headers = headers,
                     showindex=False,
                     tablefmt = 'psql',
                     floatfmt = ".2e"))

# Plotting
from matplotlib import pyplot as plt
plt.title('Time')
for meas in data.total_time:
    #plt.plot(data['size'], data.total_time[meas], label = meas, linestyle = 'None', marker = '+')
    plt.plot(data['size_MB'], data.total_time[meas]/data.repeats, label = meas)
   
plt.xscale('log')
plt.xlabel("Result array size (MB)")
plt.yscale('log')
plt.ylabel("Time (s)")

plt.grid()
plt.legend()

no_of_arrays = 3 # x,y and z

plt.figure()
plt.title('BW')
for meas in data.total_time:
    #plt.plot(data['size'], data.total_time[meas], label = meas, linestyle = 'None', marker = '+')
    plt.plot(data['size_MB'], no_of_arrays*data['size_MB']/(data.total_time[meas]/data.repeats), label = meas)

plt.xscale('log')
plt.xlabel("Result array size (MB)")
plt.yscale('log')
plt.ylabel("Bandwidth (MB/s)")
plt.legend()


plt.show()

