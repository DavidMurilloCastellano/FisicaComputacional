//Supondremos que todos los planetas parten del eje X, por lo que su velocidad inicial solo tendrá dirección Y.

//Incluimos los paquetes que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

#define G 6.67e-11
#define M 1.99e30
#define c 1.496e11


//Función reescala: transforma los datos del fichero "condiciones_iniciales.txt", dispuestos en filas según: masa,
//distancia media al Sol, velocidad orbital (ordenados según proximidad al Sol y expresados en unidades SI), en los
//correspondientes reescalados según se indica en el guion. Solo se necesita emplear la primera vez que se ejecuta 
//el programa. Los datos se han obtenido de: https://nssdc.gsfc.nasa.gov/planetary/factsheet/

void reescala(void)
{
    ifstream fichIn;
    ofstream fichOut;
    double datos[3][8], cte;
    int i,j;

    //Leemos los datos
    fichIn.open("condiciones_iniciales.txt");
    fichOut.open("datos.txt");
    while(!fichIn.eof())
    {
        if(!fichIn.is_open())
            cout << "Error al abrir el fichero";
        for (i=0; i<=2; i++)
        {
            for(j=0; j<=7; j++)
            fichIn >> datos[i][j];
        }
    }
    
    //Aplicamos la transformación
   cte=sqrt(G*M/(c*c*c));
   for(j=0;j<=7;j++)
   {
        datos[0][j]=datos[0][j]/M;
        datos[1][j]=datos[1][j]/c;
        datos[2][j]=datos[2][j]/(c*cte);
   }

    //Pasamos los datos reescalados a un nuevo fichero
    for (i=0; i<=2; i++)
        {
            for(j=0; j<=7; j++)
            fichOut << datos[i][j] << " ";
            fichOut << endl;
        }

    fichOut.close();
    fichIn.close();
    return;
    //PROBLEMA: NO HAY SUFICIENTES DECIMALES
}




int main(void)
{
    reescala();
    return 0;
}


//Hay que comprobar: órbitas son elípticas, los periodos de rotación son similares a los reales, la energía se
//conserva y las órbitas son estables frente a perturbaciones de las condiciones iniciales.