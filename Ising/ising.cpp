//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
using namespace std;

#define rng false //Indica si la configuración inicial es aleatoria (true) u ordenada (false)
#define N 20 //Número de nodos del sistema en cada eje
#define pMC 100 //Número de pasos de Monte-Carlo que se dan
#define T 0.011 //Temperatura del sistema

//Cabecera con todas las funciones que hemos definido
void conf_aleat(bool A[][N], int n);
void conf_orden(bool A[][N],int n);
//bool i2b(int x);
int b2i(bool b);
double energia(bool A[][N],int n, int m);

int main (void)
{
    int i,j,k,n,m,M,L;
    bool A[N][N];
    double p;
    ofstream fichOut;
    srand(time(NULL));

    //Elegimos una configuración inicial
    if(rng)
        conf_aleat(A,N);
    else
        conf_orden(A,N);
    
    M=N*N; L=N-1;
    fichOut.open("ising_data.dat");
    for(i=0;i<pMC;i++)
    {
        //Volcamos los datos al fichero para representarlos luego
        for(j=0;j<N;j++)
        {
            for(k=0;k<L;k++)
                fichOut << b2i(A[j][k]) << ", ";
            fichOut << b2i(A[j][L]) << endl;
        }
        fichOut << endl;

        //Elegimos un nodo al azar y aplicamos el algoritmo
        for(j=0; j<M; j++)
        {
            n=rand()%N; m=rand()%N;
            p=exp(-energia(A,n,m)/T);
            if(p>1 || rand()/RAND_MAX<p)
                if(A[n][m])
                    A[n][m]=false;
                else
                    A[n][m]=true;
        }
    }
    fichOut.close();

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

//Función energía: calcula la variación de energía correspondiente a la transición de un estado a otro
//Argumentos:A[][], vector de espines; n,m, coordenadas del nodo seleccionado
//Retorno: energía asociada a la transición
double energia(bool A[][N],int n, int m)
{
    double e;

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