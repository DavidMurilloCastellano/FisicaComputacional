# Importa los módulos necesarios
from matplotlib import pyplot as plt
from matplotlib.pyplot import cm
import numpy as np
import io

# Parámetros
# ========================================
N="128"




#Ficheros
file_in = "magnitudes-N="+N+".txt" # Nombre del fichero de datos
file_out1 = "magnetizacion-N="+N+".pdf" # Nombre del fichero de salida (sin extensión)
file_out2 = "energia-N="+N+".pdf" # Nombre del fichero de salida (sin extensión)
file_out3 = "calor-esp-N="+N+".pdf" # Nombre del fichero de salida (sin extensión)

# Lectura del fichero de datos
# ========================================
# Lee el fichero a una cadena de texto
with open(file_in, "r") as f:
    data_str = f.read()

# Inicializa la lista con los datos de cada valor de temperatura.
# frames_data[j] contiene los datos del valor de temperatura j-ésimo
frames_data = list()
T=[]

# Itera sobre los bloques de texto separados por líneas vacías
# (cada bloque corresponde a una temperatura)
for frame_data_str in data_str.split("\n\n"):
    if len(frame_data_str) > 0:
        n=frame_data_str.index("\n")
        T.append("T="+frame_data_str[:n])
        frame_data_str=frame_data_str[n+1:]
    # Almacena el bloque en una matriz
    # (io.StringIO permite leer una cadena de texto como si fuera un
    # fichero, lo que nos permite usar la función loadtxt de numpy)
        frame_data = np.loadtxt(io.StringIO(frame_data_str), delimiter=",").T

        # Añade los datos del fotograma (la configuración del sistema)
        # a la lista
        frames_data.append(frame_data)

# Almacena toda la información en un arrary de numpy
frames_data = np.array(frames_data)
fig1=plt.figure()
ax = plt.subplot(111)
x=frames_data[0][0]
color = iter(cm.rainbow(np.linspace(0, 1, len(T))))
i=0
for temp in T:
    c=next(color)
    plt.errorbar(x,frames_data[i][1],yerr=frames_data[i][2],fmt='none',capsize=2,c=c)
    i=i+1
plt.title("Magnetización promedio")
box = ax.get_position()
ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
plt.legend(T,loc='center left', bbox_to_anchor=(1, 0.5))
fig1.savefig(file_out1)

fig2=plt.figure()
ax = plt.subplot(111)
x=frames_data[0][0]
color = iter(cm.rainbow(np.linspace(0, 1, len(T))))
i=0
for temp in T:
    c=next(color)
    plt.errorbar(x,frames_data[i][3],yerr=frames_data[i][4],fmt='none',capsize=2,c=c)
    i=i+1
plt.title("Energía media")
box = ax.get_position()
ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
plt.legend(T,loc='center left', bbox_to_anchor=(1, 0.5))
fig2.savefig(file_out2)

fig3=plt.figure()
ax = plt.subplot(111)
x=frames_data[0][0]
color = iter(cm.rainbow(np.linspace(0, 1, len(T))))
i=0
for temp in T:
    c=next(color)
    plt.errorbar(x,frames_data[i][5],yerr=frames_data[i][6],fmt='none',capsize=2,c=c)
    i=i+1
plt.title("Calor específico")
box = ax.get_position()
ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
plt.legend(T,loc='center left', bbox_to_anchor=(1, 0.5))
fig3.savefig(file_out3)