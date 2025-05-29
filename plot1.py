import matplotlib.pyplot as plt
import matplotlib
from glob import glob
import numpy as np
import h5py

def read_data_from_file(file_name):
    x = []
    y1 = []
    y2 = []
    with open(file_name, 'r') as file:
        for i, line in enumerate(file, 1):
            values = line.split()
            if len(values) == 2:
                y1.append(float(values[0]))
                y2.append(float(values[1]))
                x.append(i)
    return x, y1, y2

def read_1b_data_from_file(file_name):
    x = []
    with open(file_name, 'r') as file:
        for i, line in enumerate(file, 1):
            values = line.split()
            if len(values) == 1:
                x.append(float(values[0]))
    return x
_, Y1, Y2 = read_data_from_file("outputs/caida.txt")
#Y2 = read_1b_data_from_file("outputs/dd1_erdr_hist.txt")

def display_measure_MC(file_name):
    #plt.yscale("log")
    with h5py.File(file_name, "r") as file:
        file_data = file["markovchains"]
        chains = file_data[:]
        sampling_gap = int(file_data.attrs['sampling_gap'])
        stationary_state = int(file_data.attrs['stationary_state'])

        x_range = np.arange(0, chains.shape[1], 1)
        for x in range(chains.shape[1]-1):
            if x*sampling_gap > stationary_state and stationary_state != -1:
                plt.axvspan(x*sampling_gap, (x+1)*sampling_gap, color="red", alpha=(1+x%2)*0.05)
            else:
                plt.axvspan(x*sampling_gap, (x+1)*sampling_gap, color="slategrey", alpha=(1+x%2)*0.05)
        for chain_val in chains:
            plt.plot(x_range*sampling_gap, chain_val, alpha=10/chains.shape[0], color="forestgreen")

y1 = np.array(Y1)
y2 = np.array(Y2)

display_measure_MC("outputs/markov_chains/jazzHauteRes.hdf5")



#plt.hist(y1, bins=20, density = True, edgecolor="white",alpha=0.7)
#plt.hist(y2, bins=20, density = True, edgecolor="white",alpha=0.7)

#plt.figure(figsize=(6,5))
#plt.violinplot([y2, y1], showmedians=True)
#plt.xticks([1, 2], ["Degré homogène", "Degré hétérogène"])
#plt.ylabel("Nombre d'essais")
#plt.title("Distribution des 1000 essais")

#plt.plot(y1, y2);

#file_name = 'outputs/50etoile.txt'
#x, y1, y2 = read_data_from_file(file_name)
#plt.bar(x, y1, label='repeatedSortEdgetab', color='b', alpha=0.7)
#plt.bar(x, y2, label='iterSortEdgetab', color='r', alpha=0.7, bottom=y1)

plt.legend()

plt.show()