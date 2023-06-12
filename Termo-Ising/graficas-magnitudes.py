# Importa los módulos necesarios
from matplotlib import pyplot as plt
from matplotlib.pyplot import cm
import numpy as np
import io

# Parámetros
# ========================================
N="16"



#Ficheros
file_in = "magnitudes-N="+N+".txt" # Nombre del fichero de datos
file_out1 = "magnetizacion-N="+N+".pdf" # Nombre del fichero de salida
file_out2 = "energia-N="+N+".pdf" # Nombre del fichero de salida
file_out3 = "calor-esp-N="+N+".pdf" # Nombre del fichero de salida

# Lectura del fichero de datos
# ========================================
# Lee el fichero a una cadena de texto
with open(file_in, "r") as f:
    data_str = f.read()
# frames_data[j] contiene los datos del valor de temperatura j-ésimo
frames_data = list()
# Almacena toda la información en un array de numpy
frames_data = np.array(np.loadtxt(io.StringIO(data_str), delimiter=",").T)

#Magnetización
fig1=plt.figure()
x=frames_data[0]
y=frames_data[1]
plt.errorbar(x,y,yerr=frames_data[2],capsize=2,ecolor="k")
plt.ylabel("Magnetización promedio")
plt.xlabel("Temperatura")
fig1.savefig(file_out1)

#Energía
fig2=plt.figure()
x=frames_data[0]
y=frames_data[3]
plt.errorbar(x,y,yerr=frames_data[4],capsize=2,ecolor="k")
plt.ylabel("Energía media")
plt.xlabel("Temperatura")
fig2.savefig(file_out2)

#Calor específico
fig3=plt.figure()
x=frames_data[0]
y=frames_data[5]
plt.plot(x,y,marker='o',mec="k",mfc="k")
plt.ylabel("Calor específico")
plt.xlabel("Temperatura")
fig3.savefig(file_out3)

"""
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
"""