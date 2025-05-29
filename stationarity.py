import numpy as np
from scipy.stats import ks_2samp, gaussian_kde
import argparse, h5py
import matplotlib.pyplot as plt

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

    # Comparaison paire d'étapes consécutives
    for i in range(1, steps.shape[0]):
        sample_prev = steps[i - 1]
        sample_curr = sorted(steps[i])
        KDE = gaussian_kde(sample_curr)
        #plt.hist(sample_curr, bins=100, density = True, edgecolor="white",alpha=0.7, range=(0, max(chains[0])*1.1))
        plt.xlim(7500, max(chains[0])*1.1)
        plt.plot(sample_curr, KDE(sample_curr))
        plt.show()

        stat, p_value = ks_2samp(sample_prev, sample_curr)
        #stat proche de 0->distribucion sont prochas
        #contrairement a p_value si proche de 0 ->distribucion differentes
        results.append((i-1, i, stat, p_value))
        if p_value < alpha:
            print(f"{i-1} et {i} : KS stat = {stat:.4f}, p-value = {p_value:.4f} ->","H0 rejettée")
        else :
            print(f"{i-1} et {i} : KS stat = {stat:.4f}, p-value = {p_value:.4f} ->","impossible de rejetter H0")
            break

    return results

results = ks_test(steps)
print(len(results))

file_data.attrs.modify('stationary_state',slen(results)*sampling_gap);