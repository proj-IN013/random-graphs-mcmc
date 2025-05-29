from glob import glob
import numpy as np
import argparse, h5py

parser = argparse.ArgumentParser()
required = parser.add_argument_group('required arguments')
required.add_argument('-n', '--name', type=str, help='name of the future file', required=True)
required.add_argument('-s', '--sampling-gap', type=str, help='sampling gap chosen for the experiment', required=True)
parser.add_argument('-ip', '--input-path', type=str, help='path of the input folder', default='outputs/threads/')
parser.add_argument('-op', '--output-path', type=str, help='path for the compiled result', default='outputs/markov_chains/')
args = parser.parse_args()

files = glob(args.input_path + "*.txt")
chains = []
for i in range(len(files)):
    chain = []
    with open(files[i], 'r') as file:
        for i, line in enumerate(file):
            val = line.split()
            if len(val) == 1:
                chain.append(float(val[0]))
        chains.append(chain)
chains = np.array(chains)

with h5py.File(args.output_path + args.name + ".hdf5", "w") as output_file:
    data = output_file.create_dataset("markovchains", data=chains)
    data.attrs['sampling_gap'] = args.sampling_gap
    data.attrs['stationary_state'] = -1