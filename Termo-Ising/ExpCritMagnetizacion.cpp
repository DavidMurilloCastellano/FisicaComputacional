//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
//#include <string>
#include "gsl_rng.h" //Libreria para generación de números aleatorios
using namespace std;

#define N 2048 //Número de nodos del sistema en cada eje
#define pMC 1e4 //Número de pasos de Monte-Carlo que se dan para calcular cada promedio de magnitudes
#define Tc 2.26918531421 //Temperatura crítica en las unidades empleadas según: https://en.wikipedia.org/wiki/Square_lattice_Ising_model
#define errT 0.5 //Paso entre las sucesivas temperaturas consideradas

//Cabecera con todas las funciones que hemos definido
void conf_aleat(bool A[][N],int n, gsl_rng *tau);
int b2i(bool b);
int DEnergia(bool A[][N],int n, int m);
double magn(bool A[][N], int M);

int main (void)
{
    int M, L, i, j, k, n, m, e;
    bool A[N][N];
    double T, p, sMag, mMag, mgn, eT;
    long double sMag2, varmag;
    ofstream fichO, fichG;

    //Para la generación de números aleatorios con GSL
    gsl_rng *tau;
    int semilla=time(NULL); //Semilla del generador de números aleatorios
    tau=gsl_rng_alloc(gsl_rng_taus); //Inicializamos el puntero
    gsl_rng_set(tau,semilla); //Inicializamos la semilla
    
    //Cálculo de constantes
    M=N*N; L=N-1;
    fichO.open("ln(m)-ln(T-Tc).txt");
    fichG.open("m-T.txt");
    
    //Aplicamos el algoritmo de Monte-Carlo
    k=0;
    eT=0.8;
    do
    {
        k++;
        eT=eT*errT;
        T=Tc-eT;

        //Partimos de una configuración ordenada
        for(i=0;i<N;i++)
            for(j=0;j<N;j++)
                A[i][j]=true;

        //Inicializamos la suma
        sMag=0.0;
        sMag2=0.0; 
        for(i=0;i<pMC;i++)
        {
            //Elegimos un nodo al azar y aplicamos el algoritmo durante 1 pMC
            for(j=0; j<M; j++)
            {
                n=gsl_rng_uniform_int(tau,N); m=gsl_rng_uniform_int(tau,N);
                e=DEnergia(A,n,m);
                p=exp(-e/T);
                if(p>1 || gsl_rng_uniform(tau)<p)
                {
                    if(A[n][m])
                        A[n][m]=false;
                    else
                        A[n][m]=true;
                 }
            }
            //Sumamos
            mgn=magn(A,M);
            sMag=sMag+mgn; sMag2=sMag2+mgn*mgn;
        }

        //Calculamos la magnetización promedio
        mMag=sMag/pMC;
        varmag=sMag2/pMC-mMag*mMag;       

        //Pasamos los resultados a un fichero
        fichO << log(eT) << ", " << log(mMag) << endl;
        fichG << T << ", " << mMag << ", " << sqrt(varmag/pMC) << endl;        
    } while (k<=6);

    fichO.close();
    fichG.close();

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
//Retorno: valor de la magnetización (por N^2)
double magn(bool A[][N], int M)
{
    int i,j;
    double sum;

    sum=0.0;
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            sum=sum+b2i(A[i][j]);
    
    return abs(sum)/M;
}
