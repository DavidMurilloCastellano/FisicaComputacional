# Importa los módulos necesarios
from matplotlib import pyplot as plt
from matplotlib.pyplot import cm
import numpy as np
import io

# Parámetros
# ========================================
N="32"
N1=""
N2=""
N3=""


#Ficheros
file_in = "magnitudes-N="+N+".txt" # Nombre del fichero de datos
file_in1 = "magnitudes-N="+N1+".txt" # Nombre del fichero de datos
file_in2 = "magnitudes-N="+N2+".txt" # Nombre del fichero de datos
file_in3 = "magnitudes-N="+N3+".txt" # Nombre del fichero de datos
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

if(N1!=""):
    # Lee el fichero a una cadena de texto
    with open(file_in1, "r") as f:
        data_str = f.read()
    # frames_data[j] contiene los datos del valor de temperatura j-ésimo
    frames_data1 = list()
    # Almacena toda la información en un array de numpy
    frames_data1 = np.array(np.loadtxt(io.StringIO(data_str), delimiter=",").T)

if(N2!=""):
    # Lee el fichero a una cadena de texto
    with open(file_in2, "r") as f:
        data_str = f.read()
    # frames_data[j] contiene los datos del valor de temperatura j-ésimo
    frames_data2 = list()
    # Almacena toda la información en un array de numpy
    frames_data2 = np.array(np.loadtxt(io.StringIO(data_str), delimiter=",").T)

if(N3!=""):
    # Lee el fichero a una cadena de texto
    with open(file_in3, "r") as f:
        data_str = f.read()
    # frames_data[j] contiene los datos del valor de temperatura j-ésimo
    frames_data3 = list()
    # Almacena toda la información en un array de numpy
    frames_data3 = np.array(np.loadtxt(io.StringIO(data_str), delimiter=",").T)

#Magnetización
fig1=plt.figure()
x=frames_data[0]
y=frames_data[1]
plt.errorbar(x,y,yerr=frames_data[2],capsize=2,c="b",label="N="+N)
if(N1!=""):
    plt.errorbar(x,frames_data1[1],yerr=frames_data1[2],capsize=2,c="g",label="N="+N1)
if(N2!=""):
    plt.errorbar(x,frames_data2[1],yerr=frames_data2[2],capsize=2,c="r",label="N="+N2)
if(N3!=""):
    plt.errorbar(x,frames_data3[1],yerr=frames_data3[2],capsize=2,c="y",label="N="+N3)
plt.ylabel("Magnetización promedio")
plt.xlabel("Temperatura")
if(N1!=""):
    plt.legend()
fig1.savefig(file_out1)

#Energía
fig2=plt.figure()
x=frames_data[0]
y=frames_data[3]
plt.errorbar(x,y,yerr=frames_data[4],capsize=2,c="b",label="N="+N)
if(N1!=""):
    plt.errorbar(x,frames_data1[3],yerr=frames_data1[4],capsize=2,c="g",label="N="+N1)
if(N2!=""):
    plt.errorbar(x,frames_data2[3],yerr=frames_data2[4],capsize=2,c="r",label="N="+N2)
if(N3!=""):
    plt.errorbar(x,frames_data3[3],yerr=frames_data3[4],capsize=2,c="y",label="N="+N3)
plt.ylabel("Energía media")
plt.xlabel("Temperatura")
if(N1!=""):
    plt.legend()
fig2.savefig(file_out2)

#Calor específico
fig3=plt.figure()
x=frames_data[0]
y=frames_data[5]
plt.errorbar(x,y,yerr=frames_data[6],capsize=2,c="b",label="N="+N)
#plt.plot(x,y,marker='o',c="b",label="N="+N)
if(N1!=""):
    plt.plot(x,frames_data1[5],marker="o",c="g",label="N="+N1)
if(N2!=""):
    plt.plot(x,frames_data2[5],marker="o",c="r",label="N="+N2)
if(N3!=""):
    plt.plot(x,frames_data3[5],marker="o",c="y",label="N="+N3)
plt.ylabel("Calor específico")
plt.xlabel("Temperatura")
if(N1!=""):
    plt.legend()
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