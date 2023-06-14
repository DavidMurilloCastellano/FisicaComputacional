from matplotlib import pyplot as plt
import scipy.optimize as opt
import numpy as np
import io
from math import exp

#Ficheros
file_in1 = "ln(m)-ln(T-Tc).txt" # Nombre del fichero de datos a ajustar
file_in2= "m-T.txt" # Nombre del fichero de datos con la nube de puntos a graficar
file_out1 = "ln(m)-ln(T-Tc).pdf" # Nombre del fichero de salida con el ajuste
file_out2= "m-T.pdf" # Nombre del fichero de salida con la nube de puntos
file_fit="ExpCritMagn_fit.txt"

#Función a ajustar
def func(x, a, b):
     return a*x+b

def func2(x,a,b):
    return exp(b)*pow(2.26918531421-x,a)

# Lectura del fichero de datos
# ========================================
# Lee el fichero a una cadena de texto
with open(file_in1, "r") as f:
    data_str = f.read()

# frames_data[j] contiene los datos del valor de temperatura j-ésimo
frames_data = list()
# Almacena toda la información en un array de numpy
frames_data = np.array(np.loadtxt(io.StringIO(data_str), delimiter=",").T)
x=frames_data[0]
y=frames_data[1]
fig1=plt.figure()
plt.errorbar(x,y,yerr=frames_data[2],fmt="none",capsize=2,c='k',label="Datos")
#Ajustamos a la curva
optimizedParameters, pcov = opt.curve_fit(func, x, y)
plt.plot(x, func(x, *optimizedParameters), label="Ajuste")
plt.xlabel("ln$(T_c-T)$")
plt.ylabel("ln$(m)$")
#plt.title("m="+str(optimizedParameters[0])+", n="+str(optimizedParameters[1]))
plt.legend()
fig1.savefig(file_out1)

with open(file_fit, "w") as f:
    f.write(str(optimizedParameters[0])+" "+str(1.96*np.sqrt(pcov[0][0])))

#Graficamos deshaciendo el cambio de variable
with open(file_in2, "r") as f:
    data_str = f.read()
# Inicializa la lista con los datos de cada valor de temperatura.
# frames_data[j] contiene los datos del valor de temperatura j-ésimo
frames_data = list()
# Almacena toda la información en un array de numpy
frames_data = np.array(np.loadtxt(io.StringIO(data_str), delimiter=",").T)
x=frames_data[0]
y=frames_data[1]
fig2=plt.figure()
plt.errorbar(x,y,yerr=frames_data[2],fmt="none",capsize=2,c='k',label="Datos")
x=np.linspace(1.8,2.2691,100)
plt.plot(x,func2(x,optimizedParameters[0],optimizedParameters[1]), label="Ajuste")
plt.xlabel("$T$")
plt.ylabel("m")
#plt.title("Magnetización promedio frente a temperatura. N=2048")
plt.legend()
fig2.savefig(file_out2)


