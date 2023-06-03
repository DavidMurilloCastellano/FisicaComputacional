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
file_in = "particulas-v0="+v0+"_energias.txt" # Nombre del fichero de datos
file_out = "particulas-v0="+v0+"_energias.pdf" # Nombre del fichero de salida

# Lectura del fichero de datos
# ========================================
# Lee el fichero a una cadena de texto
with open(file_in, "r") as f:
    data_str = f.read()

# frames_data[j] contiene los datos del valor de temperatura j-ésimo
frames_data = list()
# Almacena toda la información en un array de numpy
frames_data = np.array(np.loadtxt(io.StringIO(data_str), delimiter=" ").T)
t=frames_data[0]
K=frames_data[1]
V=frames_data[2]
T=frames_data[3]

fig=plt.figure()
plt.plot(t,K,label="Cinética")
plt.plot(t,V,label="Potencial")
plt.plot(t,T,label="Total")
plt.legend()
fig.savefig(file_out)

