from matplotlib import pyplot as plt
import scipy.optimize as opt
import numpy as np
import io


# Parámetros
# ========================================
v0="1"


#Ficheros
file_in = "particulas-v0="+v0+"_P-T.txt" # Nombre del fichero de datos a ajustar
file_out = "particulas-v0="+v0+"_P-T.pdf" # Nombre del fichero de salida con el ajuste

#Función a ajustar
def func(x, a, b):
     return a*x+b

# Lectura del fichero de datos
# ========================================
# Lee el fichero a una cadena de texto
with open(file_in, "r") as f:
    data_str = f.read()

# frames_data[j] contiene los datos del valor de temperatura j-ésimo
frames_data = list()
# Almacena toda la información en un array de numpy
frames_data = np.array(np.loadtxt(io.StringIO(data_str), delimiter=",").T)
x=frames_data[0]
y=frames_data[1]
fig=plt.figure()
plt.plot(x,y,".",label="Datos")
#Ajustamos a la curva
optimizedParameters, pcov = opt.curve_fit(func, x, y)
plt.plot(x, func(x, *optimizedParameters), label="Ajuste")
plt.title("m="+str(optimizedParameters[0])+", n="+str(optimizedParameters[1]))
plt.xlabel("Temperatura")
plt.ylabel("Presión")
plt.legend()
fig.savefig(file_out)