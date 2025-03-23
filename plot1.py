import matplotlib.pyplot as plt
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

def unif(listetriee, rayon, x):
    return ( (listetriee >= x-rayon) & (listetriee <= x+rayon)).sum()/(2*rayon)


Y1 = read_1b_data_from_file("outputs/res1 copie.txt");
Y2 = read_1b_data_from_file("outputs/res2 copie.txt");

x  = np.arange(0, np.max(Y1+ Y2), 1)
y1 = np.array(Y1)
y2 = np.array(Y2)
np.sort(y1)
np.sort(y2)
plt.plot(x, np.array([unif(y1, 100, xi) for xi in x]) , label='erdr', color='b', alpha=0.7)
plt.plot(x, np.array([unif(y2, 100, xi) for xi in x]) , label='dd1', color='r', alpha=0.7)

#file_name = 'outputs/50etoile.txt'
#x, y1, y2 = read_data_from_file(file_name)
#plt.bar(x, y1, label='repeatedSortEdgetab', color='b', alpha=0.7)
#plt.bar(x, y2, label='iterSortEdgetab', color='r', alpha=0.7, bottom=y1)

plt.legend()

plt.show()