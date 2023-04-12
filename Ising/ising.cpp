//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
using namespace std;

#define N 5 //Número de nodos del sistema en cada eje
#define pMC 100 //Número de pasos de Monte-Carlo que se dan
#define T 1e-4 //Temperatura del sistema

//Cabecera con todas las funciones que hemos definido
void conf_aleat(bool A[][N], int n);
void conf_orden(bool A[][N],int n);
double energia(bool A[][N],int n, int m);

int main (void)
{
    int i,j,n,m,M;
    bool A[N][N];
    double p;
    srand(time(NULL));

    //Elegimos una configuración inicial
    conf_aleat(A,N);
    M=N*N;

    //Elegimos un nodo al azar y aplicamos el algoritmo
    for(i=0;i<pMC;i++)
    {
        for(j=0; j<M; j++)
        {
            n=rand()%N; m=rand()%N;
            p=energia(A,n,m);
            //Comprobamos si es menor que 1
            if(p>1)
                p=1;
            //if(rand()/RAND_MAX<p)
                //Cambio el signo
        }
    }
    

    return 0;
}


//Función conf_aleat: genera una distribución aleatoria de espines para la configuración inicial del sistema
//Argumentos: A[][N], matriz con la configuración inicial (T=1; F=-1); n, número de nodos por eje
void conf_aleat(bool A[][N],int n)
{
    int i,j;

    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            if(rand()%2==0)
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