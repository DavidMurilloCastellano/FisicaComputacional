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