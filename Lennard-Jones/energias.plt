#
# Representacion T vs angulo
# Practica 00: Pendulo simple
#

set terminal wxt

#Título de los ejes x e y
set xlabel "t (u.a.)"
set ylabel "E (u.a.)"

#Para que no pinte la leyenda
set key outside horizontal center top width -4 height 1

#set yrange [0,7000]

#Dibuja el archivo "datos.txt" utilizando las columnas 1 (coord. x), 2 (coord. y), 3 (error x) y 4 (error y)
#Si el archivo "datos.txt" no está en la misma carpeta que el archivo con estos códigos, habrá que especificar la ruta completa: plot 'C\...\datos.txt' u 1:2:3:4 w xyerr
plot 'particulas_energias.txt' u 1:2 title "Cinética", 'particulas_energias.txt' u 1:3 title "Potencial", 'particulas_energias.txt' u 1:4 title "Total"

#Crea el archivo "datos.pdf": un pdf de la gráfica
set terminal pdf                    
set output "energia-tiempo.pdf"

#vuelve a dibujar la grafica en pantalla
set size 1,1 #Lo pone a esta escala
replot                                    

                                        

set terminal wxt
