import matplotlib.pyplot as plt
import matplotlib
from glob import glob
import numpy as np
import h5py
import matplot2tikz

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
#_, Y1, Y2 = read_data_from_file("outputs/caida.txt")
#Y2 = read_1b_data_from_file("outputs/dd1_erdr_hist.txt")


def plot_nb_essais_1000_exec():
    Y1 = read_1b_data_from_file("outputs/dd1_erdr_hist.txt")
    Y2 = read_1b_data_from_file("outputs/dd1_hist.txt")

    y1 = np.array(Y1)
    y2 = np.array(Y2)

    plt.figure(figsize=(6,3))
    plt.boxplot([y2, y1], showfliers=False, vert=False)
    plt.yticks([2, 1], ["Degré homogène", "Degré hétérogène"])
    plt.xlabel("Nombre d'essais")
    matplot2tikz.save("rendu/plot_nb_essais_1000_exec.tikz",extra_axis_parameters={
        "width=8cm",
        "height=4cm",
        "scale only axis"
    })

def distrib():
    _, X1, Y1 = read_data_from_file("data/dd1.txt")
    _, X2, Y2 = read_data_from_file("data/testerdr/erdrDD1.txt")
    X1, Y1, X2, Y2 = np.array(X1), np.array(Y1), np.array(X2)+1, np.array(Y2)

    plt.figure(figsize=(8,3))
    plt.ylim((-50,730))
    plt.plot(X1, Y1, label="$G_1$", markevery=[0,len(X1)-1], marker='|')
    plt.plot(X2, Y2, label="$G_2$", color="red", markevery=[0,len(X2)-1], marker='|')
    plt.xlabel("Degré")
    plt.ylabel("Occurences", rotation=0, labelpad=20.0)
    plt.legend(loc='upper right',
               fancybox=False, ncol=1, columnspacing=1)
    #plt.show()
    matplot2tikz.save("rendu/plot_distrib_dd1_erdr.tikz",extra_axis_parameters={
        "width=8cm",
        "height=4cm",
        "scale only axis"
    })

def swaps_successes_losses():
    Y1 = read_1b_data_from_file("outputs/successswaps/successes.txt")
    Y2 = read_1b_data_from_file("outputs/successswaps/losses.txt")

    y1 = np.array(Y1)
    y2 = np.array(Y2)

    plt.hist(y1, bins=90, density = False, edgecolor="black",alpha=0.7,)
    #plt.hist(y2, bins=90, density = False, edgecolor="black",alpha=0.7, range=(0, 2742))
    plt.show()


def display_measure_MC(file_name):
    #plt.yscale("log")
    with h5py.File(file_name, "r") as file:
        file_data = file["markovchains"]
        chains = file_data[:]
        sampling_gap = 1214 # écart d'échantillonnage classique
        step_size = int(file_data.attrs['sampling_gap']) # écart modifiable pour une courbe plus précise
        stationary_state = 9712#int(file_data.attrs['stationary_state'])

        plt.xlim([0, sampling_gap*(int((chains.shape[1]-1)*step_size/sampling_gap)-1)])
        x_range = np.arange(0, chains.shape[1], 1)
        for x in range(int((chains.shape[1]-1)*step_size/sampling_gap)):
            if x*sampling_gap > stationary_state and stationary_state != -1:
                plt.axvspan(x*sampling_gap, (x+1)*sampling_gap, color="red", alpha=(1+x%2)*0.05)
            else:
                plt.axvspan(x*sampling_gap, (x+1)*sampling_gap, color="slategrey", alpha=(1+x%2)*0.05)
        for chain_val in chains[:10]:
            plt.plot(x_range*step_size, chain_val, alpha=10/chains.shape[0], color="#457b9d")

    plt.xlabel("k-swaps effectués")
    plt.ylabel("triangles")
    plt.savefig("triangles_conv.pdf")
   #plt.show()

    #matplot2tikz.save("rendu/trig_jazz_conv.tikz",extra_axis_parameters={
    #    "width=8cm",
    #    "height=4cm",
    #    "scale only axis"
    #})


display_measure_MC("outputs/markov_chains/jazzHauteRes.hdf5")
#
#swaps_successes_losses()


#plt.hist(y1, bins=20, density = True, edgecolor="white",alpha=0.7)
#plt.hist(y2, bins=20, density = True, edgecolor="white",alpha=0.7)

#plot_nb_essais_1000_exec()
#distrib()
#plt.show()