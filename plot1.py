import matplotlib.pyplot as plt
import matplotlib
from glob import glob
import numpy as np

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

y1 = np.array(Y1)
y2 = np.array(Y2)

names = glob("outputs/threads/*.txt")

x = np.arange(0, 51, 1)


plt.yscale("log")
for i in range(len(names)):
    y =  read_1b_data_from_file(names[i])
    plt.plot(x*5000, y, alpha=1/len(x), color="blue")


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