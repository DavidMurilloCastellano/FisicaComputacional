from matplotlib import pyplot as plt
import scipy.optimize as opt
import numpy as np
import io
from math import sqrt, pi

#==========================================================================
#Parámetros
v0="1"

#=========================================================================


#Ficheros
file_in= "histograma-velocidades-v0="+v0+".txt" # Nombre del fichero de datos con la nube de puntos a graficar
file_temp= "particulas-v0="+v0+"_temperatura.txt" # Nombre del fichero con la temperatura calculada para el sistema
file_out1 = "hist-vel-X-v0="+v0+".pdf" # Nombre del fichero de salida
file_out2= "hist-vel-Y-v0="+v0+".pdf" # Nombre del fichero de salida
file_out3= "hist-vel-Mod-v0="+v0+".pdf" # Nombre del fichero de salida

#Funciones a ajustar
def Pm(v,T):
     v2=np.multiply(v,v)
     #return np.multiply(v2,np.exp(-v2/(2*T)))*sqrt(2/(pi*pow(T,3)))
     return np.multiply(v,np.exp(-v2/(2*T)))/T

def Px(v,T):
     v2=np.multiply(v,v)
     return np.exp(-v2/(2*T))/sqrt(2*pi*T)

# Lectura del fichero de datos
# ========================================
# Obtenemos la temperatura
with open(file_temp, "r") as f:
    temp = float(f.read())

#Pasamos el fichero con la nube de puntos a una cadena de texto
with open(file_in, "r") as f:
    data_str = f.read()

graf=list()
for graf_data_str in data_str.split("\n\n"):
    if len(graf_data_str) > 0:
        graf_data = np.loadtxt(io.StringIO(graf_data_str), delimiter=",").T
        graf.append(graf_data)

# Graficamos
figx=plt.figure()
x=graf[0][0]
y=graf[0][1]
d=x[1]-x[0]
plt.bar(x,y,width=d,label="Datos",color="c")
v=np.linspace(1.1*x[0],-1.1*x[0],100)
plt.plot(v,Px(v,temp),label="Ajuste",color="m")
plt.xlabel("$v_x$")
plt.ylabel("$P(v_x)$")
plt.title("Histograma de velocidades en el eje X")
plt.legend()
figx.savefig(file_out1)

figy=plt.figure()
x=graf[1][0]
y=graf[1][1]
d=x[1]-x[0]
plt.bar(x,y,width=d,label="Datos",color="c")
v=np.linspace(1.1*x[0],-1.1*x[0],100)
plt.plot(v,Px(v,temp),label="Ajuste",color="m")
plt.xlabel("$v_y$")
plt.ylabel("$P(v_y)$")
plt.title("Histograma de velocidades en el eje Y")
plt.legend()
figy.savefig(file_out2)

figm=plt.figure()
x=graf[2][0]
y=graf[2][1]
d=x[1]-x[0]
plt.bar(x,y,width=d,label="Datos",color="c")
v=np.linspace(0,1.1*x[-1],100)
plt.plot(v,Pm(v,temp),label="Ajuste",color="m")
plt.xlabel("$v$")
plt.ylabel("$P(v)$")
plt.title("Histograma del módulo de velocidades")
plt.legend()
figm.savefig(file_out3)