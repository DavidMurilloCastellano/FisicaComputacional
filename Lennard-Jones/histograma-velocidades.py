from matplotlib import pyplot as plt
import scipy.optimize as opt
import numpy as np
import io
from math import sqrt, pi

#==========================================================================
#Parámetros
v0="1"
T=0
N=20

#=========================================================================


#Ficheros
if T==0:
    file_in= "histograma-velocidades-v0="+v0+".txt" # Nombre del fichero de datos con la nube de puntos a graficar
    file_in2="pos-vel_iniciales.txt" #Nombre del fichero con velocidades iniciales
    file_temp= "particulas-v0="+v0+"_temperatura.txt" # Nombre del fichero con la temperatura calculada para el sistema
    file_out1 = "hist-vel-X-v0="+v0+".pdf" # Nombre del fichero de salida
    file_out2= "hist-vel-Y-v0="+v0+".pdf" # Nombre del fichero de salida
    file_out3= "hist-vel-Mod-v0="+v0+".pdf" # Nombre del fichero de salida
else:
    file_in= "histograma-velocidades-v0="+v0+"_temp-T="+str(T)+".txt" # Nombre del fichero de datos con la nube de puntos a graficar
    file_out1 = "hist-vel-X-v0="+v0+"_temp-T="+str(T)+".pdf" # Nombre del fichero de salida
    file_out2= "hist-vel-Y-v0="+v0+"_temp-T="+str(T)+".pdf" # Nombre del fichero de salida
    file_out3= "hist-vel-Mod-v0="+v0+"_temp-T="+str(T)+".pdf" # Nombre del fichero de salida

#Funciones a ajustar
def Pm(v,T):
     v2=np.multiply(v,v)
     return np.multiply(v,np.exp(-v2/(2*T)))/T

def Px(v,T):
     v2=np.multiply(v,v)
     return np.exp(-v2/(2*T))/sqrt(2*pi*T)

# Lectura del fichero de datos
# ========================================
# Obtenemos la temperatura (si no se ha calentado la simulación, en tal caso, las pasamos manualmente)
if T==0:
    with open(file_temp, "r") as f:
        temp = float(f.read())
else:
    temp=T

#Pasamos el fichero con la nube de puntos a una cadena de texto
with open(file_in, "r") as f:
    data_str = f.read()

graf=list()
for graf_data_str in data_str.split("\n\n"):
    if len(graf_data_str) > 0:
        graf_data = np.loadtxt(io.StringIO(graf_data_str), delimiter=",").T
        graf.append(graf_data)

with open(file_in2, "r") as f:
    inic_str = f.read()

inic_data= np.loadtxt(io.StringIO(inic_str), delimiter=" ").T
vx=np.zeros(len(graf[0][0]))
vy=np.zeros(len(graf[1][0]))
vm=np.zeros(len(vx))

# Graficamos
figx=plt.figure()
x=graf[0][0]
y=graf[0][1]
d=x[1]-x[0]
plt.bar(x,y,width=d,label="Datos",color="c")

for v0x in inic_data[2]:
    i=0
    while v0x>x[i]+d/2:
        i=i+1
    vx[i]=vx[i]+1/N
plt.plot(x,vx,".",label="Inicial",color="k")

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

for v0y in inic_data[3]:
    i=0
    while v0y>x[i]+d/2:
        i=i+1
    vy[i]=vy[i]+1/N
plt.plot(x,vy,".",label="Inicial",color="k")

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

inic_vm=np.sqrt(np.multiply(inic_data[2],inic_data[2])+np.multiply(inic_data[3],inic_data[3]))
for v0m in inic_vm:
    i=0
    while v0m>x[i]+d/2:
        i=i+1
    vm[i]=vm[i]+1/N
plt.plot(x,vm,".",label="Inicial",color="k")

v=np.linspace(0,1.1*x[-1],100)
plt.plot(v,Pm(v,temp),label="Ajuste",color="m")
plt.xlabel("$v$")
plt.ylabel("$P(v)$")
plt.title("Histograma del módulo de velocidades")
plt.legend()
figm.savefig(file_out3)