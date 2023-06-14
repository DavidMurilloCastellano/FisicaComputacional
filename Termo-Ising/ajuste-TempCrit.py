from matplotlib import pyplot as plt
import scipy.optimize as opt
import numpy as np
import io
from math import exp

#Ficheros
file_in1 = "temp-critica.txt" # Nombre del fichero de datos a ajustar
file_out1 = "temp-critica.pdf" # Nombre del fichero de salida con el ajuste
file_fit= "temp-critica_fit.txt" # Nombre del fichero de salida con la nube de puntos

#Función a ajustar
def func(x, a, b):
     return a*x+b


# Lectura del fichero de datos
# ========================================
# Lee el fichero a una cadena de texto
with open(file_in1, "r") as f:
    data_str = f.read()

# frames_data[j] contiene los datos del valor de temperatura j-ésimo
frames_data = list()
# Almacena toda la información en un array de numpy
frames_data = np.array(np.loadtxt(io.StringIO(data_str), delimiter=",").T)
x=frames_data[0]
for i in range(len(x)):
     x[i]=1/x[i]
y=frames_data[1]

fig1=plt.figure()
plt.plot(x,y,".",label="Datos")
#Ajustamos a la curva
optimizedParameters, pcov = opt.curve_fit(func, x, y)
plt.plot(x, func(x, *optimizedParameters), label="Ajuste")
plt.xlabel("$1/N$")
plt.ylabel("$T$")
plt.legend()
fig1.savefig(file_out1)

with open(file_fit, "w") as f:
     f.write(str(optimizedParameters[1])+" "+str(1.96*np.sqrt(pcov[1][1])))
