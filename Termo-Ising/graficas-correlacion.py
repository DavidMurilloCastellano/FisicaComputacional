# Importa los módulos necesarios
from matplotlib import pyplot as plt
import scipy.optimize as opt
from matplotlib.pyplot import cm
import numpy as np
import io
from math import exp

# Parámetros
# ========================================
N="2048"



#Ficheros
file_in = "correlacion-N="+N+".txt" # Nombre del fichero de datos
file_outD = "correlacion-N="+N+".pdf" # Nombre del archivo con la gráfica de la nube de puntos
file_outF = "fit_correlacion-N="+N+".pdf" # Nombre del archivo con la gráfica del ajuste
file_outfit= "fit_correlacion-N="+N+".txt" #Nombre del fichero con los parámetros del ajuste

#Función a ajustar
def func(x, a, b, c):
     return a+b*exp(-x/c)

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

# Almacena toda la información en un array de numpy
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
plt.title("Función de correlación (N="+N+")")
box = ax.get_position()
ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
plt.legend(T,loc='center left', bbox_to_anchor=(1, 0.5))
fig1.savefig(file_outD)

fig2=plt.figure()
ax = plt.subplot(111)
x=frames_data[0][0]
color = iter(cm.rainbow(np.linspace(0, 1, len(T))))
a=[]
b=[]
c=[]
i=0
for temp in T:
    c=next(color)
    y=frames_data[i][1]
    plt.errorbar(x,y,yerr=frames_data[i][2],fmt='none',capsize=2,c=c)
    optimizedParameters, pcov = opt.curve_fit(func, x, y)
    plt.plot(x, func(x, *optimizedParameters),c=c)
    a.append(optimizedParameters[0])
    b.append(optimizedParameters[1])
    c.append(optimizedParameters[2])
    i=i+1
plt.title("Cálculo de la longitud de correlación (N="+N+")")
box = ax.get_position()
ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
plt.legend(T,loc='center left', bbox_to_anchor=(1, 0.5))
fig2.savefig(file_outF)

# Pasamos a un fichero la info del ajuste
with open(file_outfit, 'w') as f:
    for j in range(len(T)):
        f.write(str(a[j])+' '+str(b[j])+' '+str(c[j])+'\n')