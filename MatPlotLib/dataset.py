import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('dataset.txt')
x = data[: , 0]

y = data[: , 1]

z = data[: , 2]


fig = plt.figure()

ax = fig.add_subplot(111)

ax.plot(x, y)
plt.show( )