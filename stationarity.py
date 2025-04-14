import numpy as np
import argparse, h5py

parser = argparse.ArgumentParser()
required = parser.add_argument_group('required arguments')
required.add_argument('-n', '--name', type=str, help='name of the file', required=True)
parser.add_argument('-ip', '--input-path', type=str, help='path', default='outputs/markov_chains/')
args = parser.parse_args()

file_path = args.input_path + args.name + ".hdf5"
with h5py.File(file_path, "r") as file:
    file_data = file["markovchains"]
    chains = file_data[:]
    sampling_gap = int(file_data.attrs['sampling_gap'])
    stationary_state = int(file_data.attrs['stationary_state'])

steps = np.transpose(chains)
print(steps)