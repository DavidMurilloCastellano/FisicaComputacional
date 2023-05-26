#
# Representacion Tcuad vs L
# Practica pendulo simple
#


set terminal wxt
#Título de los ejes
set xlabel "s (mm)"
set ylabel "1/y' (mm^{-1})"

#dibujo sin leyenda
set nokey

#en primer lugar se definen los parámetros de ajuste y se les asigna una semilla. El ajuste se hace iterativamente
a=1.0
b=1.0
#y(x) será la función de ajuste
y(x)=a*x+b

# ajusta los datos del fichero T2vsL.txt mediante los parámetros a y b
fit y(x) "ajuste.txt" via a, b

# definimos la escala de la x
#set yrange [0:0.1]
set xrange [-700:0]

#dibuja las columnas del archivo "T2vsL.txt" y la función resultante del ajuste 
plot  "ajuste.txt" u 1:2:3:4 w xyerr, y(x)

#genera la gráfica en un pdf llamado T2vsL
set terminal pdf  
set output "ajuste.pdf"

#para hacerlo uniforme sobre todo el papel, aprovechar toda la escala
set size 1,1

#redibujar
replot


set terminal wxt