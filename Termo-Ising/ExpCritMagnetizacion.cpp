//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
//#include <string>
#include "gsl_rng.h" //Libreria para generación de números aleatorios
using namespace std;

#define N 1024 //Número de nodos del sistema en cada eje
#define pMC 1e4 //Número de pasos de Monte-Carlo que se dan para calcular cada promedio de magnitudes
#define tol 1e-4 //Diferencia máxima (en valor absoluto) entre dos valores sucesivos de exponente crítico de la magnetización
#define Tc 2.26918531421 //Temperatura crítica en las unidades empleadas según: https://en.wikipedia.org/wiki/Square_lattice_Ising_model
#define errT 0.5 //Paso entre las sucesivas temperaturas consideradas

//Cabecera con todas las funciones que hemos definido
void conf_aleat(bool A[][N],int n, gsl_rng *tau);
int b2i(bool b);
int DEnergia(bool A[][N],int n, int m);
int magn(bool A[][N]);

int main (void)
{
    int M, L, i, j, k, n, m, e;
    bool A[N][N];
    double b, b0, T, p, sMag, mag, eT;
    ofstream fichO, fichOb;

    //Para la generación de números aleatorios con GSL
    gsl_rng *tau;
    int semilla=time(NULL); //Semilla del generador de números aleatorios
    tau=gsl_rng_alloc(gsl_rng_taus); //Inicializamos el puntero
    gsl_rng_set(tau,semilla); //Inicializamos la semilla
    
    //Cálculo de constantes
    M=N*N; L=N-1;
    fichO.open("ln(m)-ln(T).txt");
    fichOb.open("exp-crit_magn.txt");
    
    //Aplicamos el algoritmo de Monte-Carlo hasta alcanzar la precisión indicada
    b=0;
    k=0;
    eT=0.1;
    do
    {
        b0=b;
        k++;
        eT=eT*errT;
        T=Tc-eT;

        //Partimos de una configuración ordenada
        for(i=0;i<N;i++)
            for(j=0;j<N;j++)
                A[i][j]=true;


        //Partimos de una configuración aleatoria
//        conf_aleat(A,N,tau);

        sMag=0.0; //Inicializamos la suma
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
            sMag=sMag+magn(A);
        }

        //Calculamos la magnetización promedio
        mag=sMag/(pMC*M);

        //Calculamos el nuevo exponente crítico
        b=log(mag)/log(eT);        

        //Mostramos los resultados en pantalla
        fichO.precision(10);
        fichO << log(eT) << ", " << log(mag) << endl;
        fichOb << T << ", " << b << endl;
        
    } while (eT>=1e-6 && k<=15);

    fichO.close();
    fichOb.close();

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
int magn(bool A[][N])
{
    int i,j, sum;

    sum=0;
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            sum=sum+b2i(A[i][j]);
    
    return abs(sum);
}