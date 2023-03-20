//Supondremos que todos los planetas parten del eje X, por lo que su velocidad inicial solo tendrá dirección Y.

//Incluimos los paquetes que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

#define G 6.67e-11
#define M 1.99e30
#define c 1.496e11

//Definimos las funciones a las que luego llamaremos en el main

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
    
    //Aplicamos la transformación correspondiente a cada dato
   cte=sqrt(G*M/(c*c*c));
   for(j=0;j<=7;j++)
   {
        datos[0][j]=datos[0][j]/M;
        datos[1][j]=datos[1][j]/c;
        datos[2][j]=datos[2][j]/(c*cte);
   }

    //Pasamos los datos reescalados a un nuevo fichero
    fichOut.precision(15); //Establecemos la precisión con la que se escriben los datos en el fichero
    for (i=0; i<=2; i++)
        {
            for(j=0; j<=7; j++)
                fichOut << fixed << datos[i][j] << " ";
            fichOut << endl;
        }

    fichOut.close();
    fichIn.close();
    return;
}

//Función PosInic: lee del fichero "datos.txt" las distancias medias de cada planeta al Sol y vuelca en otro
//fichero "pos-vel_iniciales.txt" la posición y velocidad inicial de cada planeta, suponiendo que todos
//están alineados en t=0 en el eje X. En "pos-vel_iniciales.txt" aparecen en la primera fila las coordenadas de
//la posición inicial de cada planeta de la forma "x y" ordenados por proximidad al Sol. Análogo para la velocidad
//en la segunda línea (todo en las unidades reescaladas)
void PosVelInic(void)
{
    ifstream fichIn;
    ofstream fichOut;
    double datos[2][16];
    int i,j;

    //Inicializamos la matriz sabiendo que todos los planetas empiezan con y=0, v_x=0
    for(j=0;j<=14;j=j+2)
    {
        datos[0][j+1]=0.0;
        datos[1][j]=0.0;
    }

    fichIn.open("datos.txt");
    fichOut.open("pos-vel_iniciales.txt");
    while(!fichIn.eof())
    {
        if(!fichIn.is_open())
            cout << "Error al abrir el fichero";

        for(j=0;j<=7;j++)
        {
            fichIn >> datos[0][0]; //Descartamos las lecturas de las masas
        }

        for(j=0;j<=14;j=j+2)
        {
            fichIn >> datos[0][j]; //Leemos las posiciones (eje X)
        }

        for(j=1;j<=15;j=j+2)
        {
            fichIn >> datos[1][j]; //Leemos las velocidades (sentido positivo del eje Y)
        }
    }

    //Volcamos al nuevo fichero
    fichOut.precision(15);
    for(i=0;i<=1;i++)
    {
        for(j=0;j<=15;j++)
            fichOut << fixed << datos[i][j] << " ";
        fichOut << endl;
    }
    
    fichIn.close();
    fichOut.close();
    return;
}

//Función 


int main(void)
{
    //Reescalamos los datos de las condiciones iniciales
    //reescala();

    //Escribimos las posiciones y velocidades iniciales en el fichero correspondiente
    //PosVelInic();


    return 0;
}


//Hay que comprobar: órbitas son elípticas, los periodos de rotación son similares a los reales, la energía se
//conserva y las órbitas son estables frente a perturbaciones de las condiciones iniciales.