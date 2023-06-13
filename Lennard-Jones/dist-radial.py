# Importa los módulos necesarios
from matplotlib import pyplot as plt
from matplotlib.pyplot import cm
import numpy as np
import io
from math import pi

#PARÁMETROS
N=16
L=10
solido=False
gas=True


#Ficheros
file_in = "particulas_corr.txt" # Nombre del fichero de datos
file_out = "particulas_corr.pdf" # Nombre del fichero de salida

#Funciones de ajuste
A=L*L
def f1(r):
    return 2*pi*r/A
def f2(r):
    return (2*pi*r-8*np.multiply(r,np.arccos(L/(2*r))))/A

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
teo=np.zeros(15)
for i in range(6):
    teo[i]=1
for i in range(4):
    teo[i+6]=1.88
teo[10]=teo[11]=2.06
teo[12]=teo[13]=2.2361
teo[14]=2.82
z=np.zeros(len(x))

fig=plt.figure()
plt.bar(x,p,width=d,label="Datos")
if(solido):
    for r in teo:
        i=0
        while r>x[i]+d/2:
            i=i+1
        z[i]=z[i]+1/(N-1)
    k=p[0]/z[0]
    for i in range(len(z)):
        z[i]=z[i]*k
    plt.plot(x,z,c='m',label="Teoría")
    plt.legend()
if(gas):
    z1=np.linspace(1,L/2,100)
    z2=np.linspace(L/2,L/np.sqrt(2),100)
    plt.plot(z1,f1(z1),c='m',label="Teoría")
    plt.plot(z2,f2(z2),c='m')
    plt.legend()
plt.xlabel("$r$")
plt.ylabel("Densidad de partículas")
fig.savefig(file_out)

