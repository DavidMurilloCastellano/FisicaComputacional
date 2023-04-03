//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
//#include <iomanip>
using namespace std;

#define Pi 3.14159265358979 //Valor del número Pi
#define N 5 //Número de átomos que conforman nuestro sistema
#define L 10 //Tamaño (en las unidades consideradas) de cada lado de la caja cuadrada considerada
#define s 0.002 //Paso con el que se aplica el algoritmo
#define S 4 //Límite de tiempo hasta el que se considera la simulación
#define D 4 //Número de líneas que se pasan al fichero para crear el vídeo de la simulación

//Cabecera con todas las funciones que hemos definido
void cond_iniciales(int n);
double dist(double x[], double y[]);
bool ac(double a[][2], double r[][2], int n);
void rh(double r[][2], double v[][2], double a[][2], double h, int n);
void vh(double v[][2], double a[][2], double h, int n);


int main(void)
{
    int i,k;
    double a[N][2], r[N][2], v[N][2], h;
    ifstream fichIn;
    ofstream fichOut;
    bool div;
    //Generamos aleatoriamente las condiciones iniciales
    cond_iniciales(N);

    //Aplicamos el algoritmo de Verlet
    fichIn.open("pos-vel_iniciales.txt");
    while(!fichIn.eof())
    {
        if(!fichIn.is_open())
            cout << "Error al abrir el fichero";
        for(i=0;i<N;i++)
        {
            fichIn >> r[i][0];
            fichIn >> r[i][1];
            fichIn >> v[i][0];
            fichIn >> v[i][1];
        }
    }
    fichIn.close();

    fichOut.open("particulas_posiciones.txt");
    h=s; k=1;
    div=true;
    fichOut.precision(6); //Volcamos las posiciones iniciales
    for(i=0;i<N;i++)
        fichOut << r[i][0] << ", " << r[i][1] << endl;
    fichOut << endl;

    while(h<=S && div)
    {
        rh(r,v,a,s,N);
        vh(v,a,s,N);
        div=ac(a,r,N);
        vh(v,a,s,N);

        if(k%D==0) //No pasamos todas las posiciones a la simulación
        {
            for(i=0;i<N;i++)
                fichOut << r[i][0] << ", " << r[i][1] << endl;
            fichOut << endl;
        }
        h=h+s; k++;
    }
    if (!div) //Comprobamos que no ha habido errores en los cálculos
        fichOut << "Error al calcular la aceleración" << endl;

    fichOut.close();
    
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
    srand(time(NULL)); //Establecemos la semilla para la generación de números aleatorios

    //Generamos las posiciones y velocidades aleatorias
    for(i=0;i<n;i++)
    {
        v=rand()*p;
        fichOut << rand()*l << " " << rand()*l << " " << cos(v) << " " << sin(v) << endl; 
    }

    fichOut.close();    

    return;
}

//Función dist: calcula la distancia entre dos puntos de la caja teniendo en cuenta las condiciones de contorno
//periódicas.
//Argumentos: x, y; vectores de 2-dim que contienen las coordenadas de cada punto.
//Retorno: d, distancia al cubo entre los puntos dados
double dist(double x[], double y[])
{
    double d,a,b;

    a=abs(x[0]-y[0]); b=abs(x[1]-y[1]);
    if(a>L/2)
    {
        a=L-a;
        if(x[0]<y[0])
            x[0]=x[0]+L;
        else
            y[0]=y[0]+L;
    }

    if(b>L/2)
    {
        b=L-b;
        if(x[1]<y[1])
            x[1]=x[1]+L;
        else
            y[1]=y[1]+L;
    }
        
    d=sqrt(a*a+b*b);

    return d;
}

//Función ac: calcula la aceleración a la que está sometida cada partícula en un instante determinado a partir
//del potencial de Lennard-Jones.
//Argumentos: a,r; matrices de aceleración y posición de cada partícula en el instante considerado, respect.;
//n, número de partículas del sistema
//Retorno: verdadero si la distancia (denominador) no es nulo, falso en caso contrario
bool ac(double a[][2], double r[][2], int n)
{
    double f[N], d, d7, r1[2], r2[2], f0, f1;
    int i,j;
    bool div;

    f[0]=0.0; //Un cuerpo no ejerce fuerza sobre sí mismo
    div=true;
    for(i=0;i<n;i++) 
        a[i][0]=a[i][1]=0.0; //Inicializamos las sumas a 0

    for(i=0;i<n;i++)
    {
        r1[0]=r[i][0]; r1[1]=r[i][1];
        for(j=i+1;j<n;j++)
        {
            r2[0]=r[j][0]; r2[1]=r[j][1];
            d=dist(r1,r2);
            if(d<1e-15) //Comprobamos que no dividimos entre 0
            {
                a[i][0]=a[i][1]=0.0;
                j=n;
                div=false;
            }
            else if(d<3) //A distancias largas consideramos la fuerza nula
            {
                d7=pow(d,7);
                f[j]=24/d7*(2/d7-1/d); //Calculamos el módulo de la fuerza en las unidades dadas
                f0=f[j]*(r2[0]-r1[0]); //Calculamos el valor de cada componente
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

    return div;
}

//Función rh: calcula las posiciones en un instante h posterior a partir del desarrollo en serie
//Argumentos: r, v, a, matrices posiciones, velocidades y aceleraciones en el instante anterior;
//h, paso; n, número de cuerpos
void rh(double r[][2], double v[][2], double a[][2], double h, int n)
{
    int i;
    double d;

    d=h*h/2; //Calculamos las constantes fuera del bucle por optimización
    for (i=0;i<n;i++)
    {
        r[i][0]=r[i][0]+h*v[i][0]+d*a[i][0];
        //Comprobamos que las partículas no chocan con el borde de la caja
        while(r[i][0]<0) 
            r[i][0]=r[i][0]+L;
        while(r[i][0]>L) 
            r[i][0]=r[i][0]-L;

        r[i][1]=r[i][1]+h*v[i][1]+d*a[i][1];
        //Comprobamos que las partículas no chocan con el borde de la caja
        while(r[i][1]<0) 
            r[i][1]=r[i][1]+L;
        while(r[i][1]>L) 
            r[i][1]=r[i][1]-L;
    }

    return;
}

//Función vh: calcula los dos primeros sumandos de la velocidad en un instante h posterior a partir del desarrollo 
//en serie
//Argumentos: v, a, matrices posiciones, velocidades y aceleraciones en el instante anterior;
//h, paso; n, número de cuerpos
void vh(double v[][2], double a[][2], double h, int n)
{
    int i;
    double d;

    d=h/2;
    for (i=0;i<n;i++)
    {
        v[i][0]=v[i][0]+d*a[i][0];
        v[i][1]=v[i][1]+d*a[i][1];
    }

    return;
}