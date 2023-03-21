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
//Argumentos: n, número de cuerpos que consideramos en nuestro sistema
void reescala(int n)
{
    ifstream fichIn;
    ofstream fichOut;
    double datos[3][n], cte;
    int i,j;

    //Leemos los datos
    fichIn.open("condiciones_iniciales.txt");
    fichOut.open("datos.txt");
    n--;
    while(!fichIn.eof())
    {
        if(!fichIn.is_open())
            cout << "Error al abrir el fichero";
        for (i=0; i<=2; i++)
        {
            for(j=0; j<=n; j++)
                fichIn >> datos[i][j];
        }
    }
    
    //Aplicamos la transformación correspondiente a cada dato
   cte=sqrt(G*M/(c*c*c));
   for(j=0;j<=n;j++)
   {
        datos[0][j]=datos[0][j]/M;
        datos[1][j]=datos[1][j]/c;
        datos[2][j]=datos[2][j]/(c*cte);
   }

    //Pasamos los datos reescalados a un nuevo fichero
    fichOut.precision(15); //Establecemos la precisión con la que se escriben los datos en el fichero
    for (i=0; i<=2; i++)
        {
            for(j=0; j<=n; j++)
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
//Argumentos: n, número de cuerpos que consideramos en nuestro sistema
void PosVelInic(int n)
{
    ifstream fichIn;
    ofstream fichOut;
    double datos[2][2*n];
    int i,j,m;

    //Inicializamos la matriz sabiendo que todos los planetas empiezan con y=0, v_x=0
    m=2*n-1;
    n--;
    for(j=0;j<=m;j=j+2)
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

        for(j=0;j<=n;j++)
            fichIn >> datos[0][0]; //Descartamos las lecturas de las masas

        for(j=0;j<=m;j=j+2)
            fichIn >> datos[0][j]; //Leemos las posiciones (eje X)

        for(j=1;j<=m;j=j+2)
            fichIn >> datos[1][j]; //Leemos las velocidades (sentido positivo del eje Y, giro antihorario)
    }

    //Volcamos al nuevo fichero
    fichOut.precision(8);
    for(i=0;i<=1;i++)
    {
        for(j=0;j<=m;j++)
            fichOut << fixed << datos[i][j] << " ";
        fichOut << endl;
    }
    
    fichIn.close();
    fichOut.close();
    return;
}

//Función dist3: calcula la distancia al cubo entre dos puntos del espacio.
//Argumentos: x, y; vectores de 2-dim que contienen las coordenadas de cada punto.
double dist3(double x[], double y[])
{
    double d;
    d=sqrt((x[0]-y[0])*(x[0]-y[0])+(x[1]-y[1])*(x[1]-y[1]));
    d=d*d*d;
    return d;
}

//Función ac: calcula la aceleración a la que está sometida cada cuerpo en un instante determinado a partir
//de la ley de gravitación de Newton en las unidades en las que se está trabajando (G=1).
//Argumentos: a,r; vectores de aceleración y posición de cada cuerpo en el instante considerado, respectivamente;
//n, número de cuerpos del sistema
void ac(double a[], double r[], int n)
{
    double cte;
    int i,j;
    
    return;
}

int main(void)
{
    //Reescalamos los datos de las condiciones iniciales
    reescala(8);

    //Escribimos las posiciones y velocidades iniciales en el fichero correspondiente
    PosVelInic(8);


    return 0;
}


//Hay que comprobar: órbitas son elípticas, los periodos de rotación son similares a los reales, la energía se
//conserva y las órbitas son estables frente a perturbaciones de las condiciones iniciales.