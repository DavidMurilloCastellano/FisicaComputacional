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
file_in1 = "particulas_separacion.txt" # Nombre del fichero de datos
file_in2 = "particulas-v0="+v0+"_temperatura.txt" # Nombre del fichero de datos
file_out = "particulas_separacion.pdf" # Nombre del fichero de salida

# Lectura del fichero de datos
# ========================================
# Lee el fichero a una cadena de texto
with open(file_in1, "r") as f:
    data_str = f.read()

frames_data = list()
i=0
# Almacena toda la información en un array de numpy
for frame_data_str in data_str.split("\n\n"):
    if len(frame_data_str) > 0:
        frame_data = np.loadtxt(io.StringIO(frame_data_str), delimiter=",").T
        frames_data.append(frame_data)
        i=i+1
frames_data = np.array(frames_data)

with open(file_in2, "r") as f:
    data_str2 = f.read()

# frames_data[j] contiene los datos del valor de temperatura j-ésimo
temp = list()
# Almacena toda la información en un array de numpy
temp = np.array(np.loadtxt(io.StringIO(data_str2), delimiter=",").T)

t=frames_data[0][0]
x=frames_data[0][1]
l=len(t)
y=np.array([temp[0]]*l)

for j in range(i-1):
    t=np.concatenate((t,frames_data[j+1][0]))
    x=np.concatenate((x,frames_data[j+1][1]))
    y=np.concatenate((y,np.array([temp[j+1]]*l)))
    
fig=plt.figure()
plt.plot(t,x,label="Separación cuadrática")
plt.plot(t,y,label="Temperatura")
plt.xlabel("Tiempo")
plt.legend()
fig.savefig(file_out)