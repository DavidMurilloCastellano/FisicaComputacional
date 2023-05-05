# Importa los módulos necesarios
from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import io

# Parámetros
# ========================================
file_in = "magnitudes-N=16.txt" # Nombre del fichero de datos
file_out1 = "magnetizacion" # Nombre del fichero de salida (sin extensión)
file_out2 = "energia" # Nombre del fichero de salida (sin extensión)
file_out3 = "calor-esp" # Nombre del fichero de salida (sin extensión)

# Lectura del fichero de datos
# ========================================
# Lee el fichero a una cadena de texto
with open(file_in, "r") as f:
    data_str = f.read()

# Inicializa la lista con los datos de cada valor de temperatura.
# frames_data[j] contiene los datos del valor de temperatura j-ésimo
frames_data = list()

# Itera sobre los bloques de texto separados por líneas vacías
# (cada bloque corresponde a una temperatura)
for frame_data_str in data_str.split("\n\n"):
    if len(frame_data_str) > 0:
        n=frame_data_str.index("\n")
        T=frame_data_str[:n]
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
