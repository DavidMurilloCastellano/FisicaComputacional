#
# Representacion T vs angulo
# Practica 00: Pendulo simple
#

set terminal wxt

#Título de los ejes x e y
set xlabel "t (días)"
set ylabel "E (·10^{3} u)"

#Para que no pinte la leyenda
set nokey

#Dibuja el archivo "datos.txt" utilizando las columnas 1 (coord. x), 2 (coord. y), 3 (error x) y 4 (error y)
#Si el archivo "datos.txt" no está en la misma carpeta que el archivo con estos códigos, habrá que especificar la ruta completa: plot 'C\...\datos.txt' u 1:2:3:4 w xyerr
plot 'planetas_energias.txt' u 1:2:3:4 w xyerr

#Crea el archivo "datos.pdf": un pdf de la gráfica
set terminal pdf                    
set output "energias-dias2.pdf"

#vuelve a dibujar la grafica en pantalla
set size 1,1 #Lo pone a esta escala
replot                                    

                                        

set terminal wxt
