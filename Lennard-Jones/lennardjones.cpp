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
//Argumentos: a,r; matrices de aceleración y posición de cada partícula en el instante considerado, respect.;
//n, número de partículas del sistema
void ac(double a[][2], double r[][2], int n)
{
    double f[N], d, d7, r1[2], r2[2], f0, f1;
    int i,j;

    f[0]=0.0; //Un cuerpo no ejerce fuerza sobre sí mismo
    for(i=0;i<n;i++) 
        a[i][0]=a[i][1]=0.0; //Inicializamos las sumas a 0

    for(i=0;i<n;i++)
    {
        r1[0]=r[i][0]; r1[1]=r[i][1];
        for(j=i+1;j<n;j++)
        {
            r2[0]=r[j][0]; r2[1]=r[j][1];
            d=dist(r1,r2);
            if(d<3) //A distancias largas consideramos la fuerza nula
            {
                d7=pow(d,7);
                f[j]=24*(2/(d7*d7)-1/(d7*d));
                f0=f[j]*(r2[0]-r1[0]);
                f1=f[j]*(r2[1]-r1[1]);

                //Calculamos la aceleración proporcionada a la partícula j por la fuerza creada por i
                a[j][0]=a[j][0]+f0;
                a[j][1]=a[j][1]+f1;

                //Aplicamos la 3ª ley de Newton para calcular la fuerza experimentada por la partícula i
                a[i][0]=a[i][0]-f0;
                a[i][1]=a[i][1]-f1; 
            }
        }
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