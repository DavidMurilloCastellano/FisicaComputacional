//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
//#include <iomanip>
using namespace std;

#define Pi 3.14159265358979 //Valor del número Pi
#define N 20 //Número de átomos que conforman nuestro sistema
#define L 10 //Tamaño (en las unidades consideradas) de cada lado de la caja cuadrada considerada
#define s 0.002 //Paso con el que se aplica el algoritmo

//Cabecera con todas las funciones que hemos definido
void cond_iniciales(int n);
double dist(double x[], double y[]);
void ac(double a[], double r[], double m[], int n);
void rh(double r[], double v[], double a[], double h, int n);
void vh(double v[], double a[], double h, int n);


int main(void)
{
    //Generamos aleatoriamente las condiciones iniciales
    cond_iniciales(N);
    //Aplicamos el algoritmo de Verlet
    
    
    return 0;
}


//Función cond_iniciales: genera de forma aleatoria las posiciones y velocidades iniciales de las partículas en la
//caja. Escribe dicha información en el fichero "pos-vel_iniciales.txt" según: en cada línea se escribe posición en
//X, posición en Y, velocidad en X, velocidad en Y
//Argumentos: n, número de partículas
void cond_iniciales(int n)
{
    int i;
    double v,p,l;
    ofstream fichOut;

    
    v=0.0;
    p=2*Pi/RAND_MAX; //Calculamos la cte por la que hay que multiplicar para generar números aleatorios en [0,2Pi]
    l=1.0*L/RAND_MAX; //Calculamos la cte por la que hay que multiplicar para generar números aleatorios en [0,L]
    fichOut.open("pos-vel_iniciales.txt");
    fichOut.precision(8);

    //Generamos las posiciones y velocidades aleatorias
    for(i=0;i<n;i++)
    {
        v=rand()*p;
        fichOut << rand()*l << " " << rand()*l << " " << cos(v) << " " << sin(v) << endl; 
    }

    fichOut.close();    

    return;
}

//Función dist: calcula la distancia entre dos puntos del espacio.
//Argumentos: x, y; vectores de 2-dim que contienen las coordenadas de cada punto.
//Retorno: d, distancia al cubo entre los puntos dados
double dist(double x[], double y[])
{
    double d;
    d=sqrt((x[0]-y[0])*(x[0]-y[0])+(x[1]-y[1])*(x[1]-y[1]));

    return d;
}

//Función ac: calcula la aceleración a la que está sometida cada partícula en un instante determinado a partir
//del potencial de Lennard-Jones.
//Argumentos: a,r; vectores de aceleración, posición y masa de cada partícula en el instante considerado, respect;
//n, número de partículas del sistema
void ac(double a[], double r[], double m[], int n)
{
    double f[N][N], d, r1[2],r2[2];
    int i,j;

    for(i=0;i<n;i++)
    {
        
    }


    return;
}

//Función rh: calcula las posiciones en un instante h posterior a partir del desarrollo en serie
//Argumentos: r, v, a, vectores posiciones, velocidades y aceleraciones en el instante anterior;
//h, paso; n, número de cuerpos
void rh(double r[], double v[], double a[], double h, int n)
{
    int i,k;
    double d;

    k=2*n;
    d=h*h/2; //Calculamos las constantes fuera del bucle por optimización
    for (i=0;i<k;i=i+2)
    {
        r[i]=r[i]+h*v[i]+d*a[i];
        r[i+1]=r[i+1]+h*v[i+1]+d*a[i+1];
    }

    return;
}

//Función vh: calcula los dos primeros sumandos de la velocidad en un instante h posterior a partir del desarrollo 
//en serie
//Argumentos: v, a, vectores posiciones, velocidades y aceleraciones en el instante anterior;
//h, paso; n, número de cuerpos
void vh(double v[], double a[], double h, int n)
{
    int i,k;
    float d;

    k=2*n;
    d=h/2;
    for (i=0;i<k;i=i+2)
    {
        v[i]=v[i]+d*a[i];
        v[i+1]=v[i+1]+d*a[i+1];
    }

    return;
}