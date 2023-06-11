# Importa los módulos necesarios
from matplotlib import pyplot as plt
from matplotlib.pyplot import cm
import numpy as np
import io


#Ficheros
file_in = "particulas_corr.txt" # Nombre del fichero de datos
file_out = "particulas_corr.pdf" # Nombre del fichero de salida

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
p=frames_data[1]
d=x[1]-x[0]

fig=plt.figure()
plt.bar(x,p,width=d)
plt.xlabel("$r$")
plt.ylabel("Proporción de partículas")
fig.savefig(file_out)

