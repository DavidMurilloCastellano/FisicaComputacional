//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
# include "gsl_rng.h" //Libreria para generación de números aleatorios
using namespace std;

#define rng true //Indica si la configuración inicial es aleatoria (true) u ordenada (false)
#define N 128 //Número de nodos del sistema en cada eje
#define pMC 100 //Número de pasos de Monte-Carlo que se dan para calcular cada promedio de magnitudes
#define T 1.5 //Temperatura del sistema


//Cabecera con todas las funciones que hemos definido
void conf_aleat(bool A[][N],int n, gsl_rng *tau);
void conf_orden(bool A[][N],int n);
//bool i2b(int x);
int b2i(bool b);
int DEnergia(bool A[][N],int n, int m);
int magn(bool A[][N]);
int energia(bool A[][N]);
int corr(bool A[][N], int i);

int main (void)
{
    int i1,i2,j,k,n,m,M,L,e;
    bool A[N][N], x,y;
    double p, mag, E, E2, cN;
    ofstream fichODat, fichOMag;
    gsl_rng *tau;
    int semilla=time(NULL); //Semilla del generador de números aleatorios
    tau=gsl_rng_alloc(gsl_rng_taus); //Inicializamos el puntero
    gsl_rng_set(tau,semilla); //Inicializamos la semilla

    //Elegimos una configuración inicial
    if(rng)
        conf_aleat(A,N,tau);
    else
        conf_orden(A,N);
    
    M=N*N; L=N-1;
    mag=E=E2=0.0;
    fichODat.open("ising_data.dat");
    fichOMag.open("ising_magnitudes.txt");
    for(i1=0;i1<1;i1++)
    {
        for(i2=0;i2<pMC;i2++)
        {
            //Volcamos los datos al fichero para representarlos luego
            for(j=0;j<N;j++)
            {
                for(k=0;k<L;k++)
                    fichODat << b2i(A[j][k]) << ", ";
                fichODat << b2i(A[j][L]) << endl;
            }
            fichODat << endl;

            //Elegimos un nodo al azar y aplicamos el algoritmo durante 1 pMC
            for(j=0; j<M; j++)
            {
                n=gsl_rng_uniform_int(tau,N); m=gsl_rng_uniform_int(tau,N);
                p=exp(((-1.0)*DEnergia(A,n,m))/T);
                if(p>1 || gsl_rng_uniform(tau)<p)
                    if(A[n][m])
                        A[n][m]=false;
                    else
                        A[n][m]=true;
            }
/*
            //Calculamos las sumas que nos permitan hallar los promedios luego
            mag=mag+magn(A);
            e=energia(A); E=E+e; E2=E2+e*e;
*/
        }
/*
        //Cálculo de los promedios de todas las magnitudes de interés
        mag=mag/(M*pMC);
        cN=(E2-(E*E)/pMC)/(4*M*T*pMC);
        E=E/(4*N);
                
        //Pasar al fichero, ¿CÁLCULO VARIANZAS? ¿NECESITO cN?
*/
    }
    fichODat.close();
    fichOMag.close();

    return 0;
}


//Función conf_aleat: genera una distribución aleatoria de espines para la configuración inicial del sistema
//Argumentos: A[][N], matriz con la configuración inicial (T=1; F=-1); n, número de nodos por eje
void conf_aleat(bool A[][N],int n, gsl_rng *tau)
{
    int i,j;

    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            if(gsl_rng_uniform_int(tau,2)==0)
                A[i][j]=true;
            else
                A[i][j]=false;

    return;
}

//Función conf_orden: genera una distribución ordenada de espines
//Argumentos: A[][N], matriz con la configuración inicial (T=1; F=-1); n, número de nodos por eje
void conf_orden(bool A[][N],int n)
{
    int i,j;

    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            A[i][j]=true;

    return;
}
/*
//Función i2b: pasa un entero a booleano según si es positivo (true) o negativo (false)
//Argumentos: x, variable entera que se desea transformar
bool i2b(int x)
{
    if (x>0)
        return true;
    else
        return false;
}
*/
//Función b2i: devuelve 1 si el booleano es true y -1 si es false
//Argumentos: b, variable booleana que se desea transformar
int b2i(bool b)
{
    if(b)
        return 1;
    else
        return -1;
}

//Función DEnergía: calcula la variación de energía correspondiente a la transición de un estado a otro
//Argumentos:A[][], vector de espines; n,m, coordenadas del nodo seleccionado
//Retorno: energía asociada a la transición
int DEnergia(bool A[][N],int n, int m)
{
    int e;

    if(n==N-1)
        e=b2i(A[0][m]);
    else
        e=b2i(A[n+1][m]);

    if(n==0)
        e=e+b2i(A[N-1][m]);
    else
        e=e+b2i(A[n-1][m]);

    if(m==N-1)
        e=e+b2i(A[n][0]);
    else
        e=e+b2i(A[n][m+1]);

    if(m==0)
        e=e+b2i(A[n][N-1]);
    else
        e=e+b2i(A[n][m-1]);

    e=e*2*b2i(A[n][m]);

    return e;
}

//Función magn: devuelve la magnetización promedio (multiplicada por N^2 por optimización)
//Argumentos: A[][], matriz con la orientación de los espines.
//Retorno: valor de la magnetización
int magn(bool A[][N])
{
    int i,j, sum;

    sum=0;
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            sum=sum+b2i(A[i][j]);
    
    return sum;
}

//Función energia: devuelve el doble de la energía del sistema en un determinado instante
//Argumentos: A[][], matriz con la orientación de los espines, 
//Retorno: e, (doble) energía del sistema
int energia(bool A[][N])
{
    int e,i,j;

    e=b2i(A[0][0])*(b2i(A[0][1])+b2i(A[0,N-1])+b2i(A[1][0])+b2i(A[N-1][0])); //Energía nodo (0,0)

    for(j=1;j<N-1;j++)
        e=e+b2i(A[0][j])*(b2i(A[0][j+1])+b2i(A[0][j-1])+b2i(A[1][j])+b2i(A[N-1][j])); //Energía nodos (0,1),...,(0,N-2)
    
    e=e+b2i(A[0][N-1])*(b2i(A[0][0])+b2i(A[0][N-2])+b2i(A[1][N-1])+b2i(A[N-1][N-1])); //Energía nodo (0,N-1)

    for(i=1;i<N-1;i++)
    {
        e=e+b2i(A[i][0])*(b2i(A[i][1])+b2i(A[i][N-1])+b2i(A[i+1][0])+b2i(A[i-1][0])); //Energía nodo (i,0)

        for(j=1;j<N-1;j++)
            e=e+b2i(A[i][j])*(b2i(A[i][j+1])+b2i(A[i][j-1])+b2i(A[i+1][j])+b2i(A[i-1][j])); //Energía nodos (i,1),...,(i,N-2)

        e=e+b2i(A[i][N-1])*(b2i(A[i][0])+b2i(A[i][N-2])+b2i(A[i+1][N-1])+b2i(A[i-1][N-1])); //Energía nodo (i,N-1)
    }

    e=e+b2i(A[N-1][0])*(b2i(A[N-1][1])+b2i(A[N-1][N-1])+b2i(A[0][0])+b2i(A[N-2][0])); //Energía nodo (N-1,0)

    for(j=1;j<N-1;j++)
        e=e+b2i(A[N-1][j])*(b2i(A[N-1][j+1])+b2i(A[N-1][j-1])+b2i(A[0][j])+b2i(A[N-2][j])); //Energía nodos (N-1,1),...,(N-1,N-2)
    
    e=e+b2i(A[N-1][N-1])*(b2i(A[N-1][0])+b2i(A[N-1][N-2])+b2i(A[0][N-1])+b2i(A[N-2][N-1])); //Energía nodo (N-1,N-1)

    return -e;
}


//Función corr: devuelve la correlación (multiplicada por N^2 por optimización)
//Argumentos: A[][], matriz con la orientación de los espines; i, entrada de la función correlación
//Retorno: valor de la correlación
int corr(bool A[][N], int i)
{
    int sum, m,n,u;

    sum=0; u=N-1;
    if(0<=i && i<=u)
    {
        u=u-i;
        for(m=0;m<N;m++)
        {
            for(n=0;n<=u;n++)
                if(A[n][m]==A[n+i][m])
                    sum=sum+1;
                else
                    sum=sum-1;

            for(n=u+1;n<N;n++)
                if(A[n][m]==A[n+i-N][m])
                    sum=sum+1;
                else
                    sum=sum-1;
        }
    }
    
    return sum;
}
