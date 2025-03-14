import matplotlib.pyplot as plt

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

file_name = 'outputs/50etoile.txt'
x, y1, y2 = read_data_from_file(file_name)
plt.bar(x, y1, label='repeatedSortEdgetab', color='b', alpha=0.7)
#plt.bar(x, y2, label='iterSortEdgetab', color='r', alpha=0.7, bottom=y1)

plt.legend()

plt.show()