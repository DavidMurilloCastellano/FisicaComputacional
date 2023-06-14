# Importa los módulos necesarios
from matplotlib import pyplot as plt
import scipy.optimize as opt
from matplotlib.pyplot import cm
import numpy as np
import io



#Ficheros
file_in1 = "fit_correlacion-N=32.txt" #Nombre del fichero con los parámetros del ajuste
file_in2 = "fit_correlacion-N=64.txt" #Nombre del fichero con los parámetros del ajuste
file_in3 = "fit_correlacion-N=128.txt" #Nombre del fichero con los parámetros del ajuste
file_outLong="long_corr-T.pdf"


fig=plt.figure()

with open(file_in1, "r") as f:
    data_str = f.read()
frame_data = np.array(np.loadtxt(io.StringIO(data_str), delimiter=",").T)
x=frame_data[0]
y=frame_data[1]
dy=frame_data[2]
plt.errorbar(x,y,yerr=dy,capsize=2, label="N=32")

with open(file_in2, "r") as f:
    data_str = f.read()
frame_data = np.array(np.loadtxt(io.StringIO(data_str), delimiter=",").T)
x=frame_data[0]
y=frame_data[1]
dy=frame_data[2]
plt.errorbar(x,y,yerr=dy,capsize=2, label="N=64")

with open(file_in3, "r") as f:
    data_str = f.read()
frame_data = np.array(np.loadtxt(io.StringIO(data_str), delimiter=",").T)
x=frame_data[0]
y=frame_data[1]
dy=frame_data[2]
plt.errorbar(x,y,yerr=dy,capsize=2, label="N=128")

plt.xlabel("$T$")
plt.ylabel(r"$\xi$")
plt.legend()
fig.savefig(file_outLong)