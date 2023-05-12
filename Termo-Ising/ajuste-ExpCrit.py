from matplotlib import pyplot as plt
import scipy.optimize as opt
import numpy as np
import io

#Ficheros
file_in = "ln(m)-ln(T).txt" # Nombre del fichero de datos
file_out = "ln(m)-ln(T).pdf" # Nombre del fichero de salida

#Función a ajustar
def func(x, a, b):
     return a*x+b

# Lectura del fichero de datos
# ========================================
# Lee el fichero a una cadena de texto
with open(file_in, "r") as f:
    data_str = f.read()

# Inicializa la lista con los datos de cada valor de temperatura.
# frames_data[j] contiene los datos del valor de temperatura j-ésimo
frames_data = list()
# Almacena toda la información en un array de numpy
frames_data = np.array(np.loadtxt(io.StringIO(data_str), delimiter=",").T)
x=frames_data[0]
y=frames_data[1]
plt.plot(x,y,".",label="Data")
#Ajustamos a la curva
optimizedParameters, pcov = opt.curve_fit(func, x, y)
plt.plot(x, func(x, *optimizedParameters), label="Fit")
plt.title("m="+str(optimizedParameters[0])+", n="+str(optimizedParameters[1]))
plt.legend()
plt.savefig(file_out)

