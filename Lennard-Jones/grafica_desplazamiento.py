# Importa los módulos necesarios
from matplotlib import pyplot as plt
from matplotlib.pyplot import cm
import numpy as np
import io

#==========================================================================
#Parámetros
v0="0"

#=========================================================================



#Ficheros
file_in = "particulas_desplazamiento.txt" # Nombre del fichero de datos
file_out = "particulas_desplazamiento.pdf" # Nombre del fichero de salida

# Lectura del fichero de datos
# ========================================
# Lee el fichero a una cadena de texto
with open(file_in, "r") as f:
    data_str = f.read()

# frames_data[j] contiene los datos del valor de temperatura j-ésimo
frames_data = list()
# Almacena toda la información en un array de numpy
frames_data = np.array(np.loadtxt(io.StringIO(data_str), delimiter=",").T)
t=frames_data[0]
d=frames_data[1]

fig=plt.figure()
plt.plot(t,d)
plt.xlabel("Tiempo")
plt.ylabel("Desplazamiento cuadrático")
fig.savefig(file_out)

