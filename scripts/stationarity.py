import numpy as np
from scipy.stats import ks_2samp, gaussian_kde
import argparse, h5py
import matplotlib.pyplot as plt
import matplot2tikz

parser = argparse.ArgumentParser()
required = parser.add_argument_group('required arguments')
required.add_argument('-n', '--name', type=str, help='name of the file', required=True)
parser.add_argument('-ip', '--input-path', type=str, help='path', default='outputs/markov_chains/')
args = parser.parse_args()

file_path = args.input_path + args.name + ".hdf5"
file = h5py.File(file_path, "r+")
file_data = file["markovchains"]
chains = file_data[:]
sampling_gap = int(file_data.attrs['sampling_gap'])
stationary_state = int(file_data.attrs['stationary_state'])

steps = np.transpose(chains)
print(f"Dimensions des steps: {steps.shape}")
print(f"Sampling gap: {sampling_gap}")
print(f"Stationary state (avant): {stationary_state}\n")

def ks_test(steps):
    print("Dimensions des steps:", steps.shape)
    #liste pour stocker les resultats
    results = []
    alpha = .1  #seuil
    first_null = -1

    # Comparaison paire d'étapes consécutives
    for i in range(10, steps.shape[0]):
        sample_prev = sorted(steps[i - 10])
        sample_curr = sorted(steps[i])
        #plt.xlim(7500, max(chains[0])*1.1)
        plt.xlim(8300, 10000)
        #KDE = gaussian_kde(sample_curr)
        #plt.plot(sample_curr, KDE(sample_curr))
        plt.hist(sample_curr, bins=15, density = True, edgecolor="white",alpha=0.7)

        if first_null != -1:
            null_smpl = sorted(steps[first_null])
            #KDE = gaussian_kde(null_smpl)
            #plt.plot(null_smpl, KDE(null_smpl))
            plt.hist(null_smpl, bins=15, density = True, edgecolor="white",alpha=0.7)
        plt.show()

        stat, p_value = ks_2samp(sample_prev, sample_curr)
        #stat proche de 0->distribucion sont prochas
        #contrairement a p_value si proche de 0 ->distribucion differentes
        results.append((i-1, i, stat, p_value))
        if p_value < alpha:
            print(f"{i-1} et {i} : KS stat = {stat:.4f}, p-value = {p_value:.4f} ->","H0 rejettée")
        else :
            print(f"{i-1} et {i} : KS stat = {stat:.4f}, p-value = {p_value:.4f} ->","impossible de rejetter H0")
            if first_null == -1:
                first_null = i
            #break

    return results

def plot_conv_chains():
    file = h5py.File("outputs/markov_chains/jazz300k_x10.hdf5", "r+")
    file_data = file["markovchains"]
    chains = file_data[:]
    sampling_gap = int(file_data.attrs['sampling_gap'])
    stationary_state = int(file_data.attrs['stationary_state'])
    steps = np.transpose(chains)
    plt.xlim(8300, 11700)
    steps_to_plot = [6,9,13,47,2479]
    labels = ['Après 0.2m swaps   ', 'Après 0.4m swaps   ', 'Après 0.6m swaps   ', 'Convergence détéctée   ', 'Après 100m swaps   ']
    colors = ['#1d3557', '#457b9d', '#a8dadc', '#e63946', '#f1faee']
    for i, label, color in zip(steps_to_plot, labels, colors):
        sample = sorted(steps[i])
        plt.hist(sample, bins=90, density = False, edgecolor="black",alpha=0.7, label=label, color=color, range=(8300, 11700))
    plt.xlabel("Nombre de triangles")
    #plt.ylabel("Nombre de triangles")
    handles, labels = plt.gca().get_legend_handles_labels()
    plt.legend(reversed(handles), reversed(labels), loc='upper center', bbox_to_anchor=(0.5, -0.3),
               fancybox=False, ncol=3, columnspacing=1)
    plt.arrow(10850, 12, -750, 0, width=0.17, length_includes_head=True, antialiased=True, color="black", head_length=30)#, head_width=0.08*2.5)
    plt.arrow(9700, 12, -300, 0, width=0.17, length_includes_head=True, antialiased=True, color="black", head_length=30)#, head_width=0.08*2.5)
    plt.arrow(9000, 12, -200, 0, width=0.17, length_includes_head=True, antialiased=True, color="black", head_length=30)#, head_width=0.08*2.5)
    #plt.tight_layout()
    #plt.show()
    matplot2tikz.save("rendu/plot_conv_chains_V3.tikz",extra_axis_parameters={
        "width=16cm",
        "height=8cm"
        #"scale only axis"
    })

results = ks_test(steps)
print(len(results))

file_data.attrs.modify('stationary_state',len(results)*sampling_gap);