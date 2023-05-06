//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
//#include <string>
#include "gsl_rng.h" //Libreria para generación de números aleatorios
using namespace std;

#define N 128 //Número de nodos del sistema en cada eje
#define pMC 1e6 //Número de pasos de Monte-Carlo que se dan para calcular cada promedio de magnitudes
#define tol 1e-6 //Diferencia máxima (en valor absoluto) entre dos valores sucesivos de exponente crítico de la magnetización
#define T0 1e-3 //Error relativo inicial entre la temperatura crítica y la empleada en el cálculo numérico

//Cabecera con todas las funciones que hemos definido
int b2i(bool b);
int DEnergia(bool A[][N],int n, int m);
int magn(bool A[][N]);

int main (void)
{
    int M, L, i, j, k, n, m, e;
    bool A[N][N];
    double b, b0, T, p, sMag, mag;
    ofstream fichO;

    //Para la generación de números aleatorios con GSL
    gsl_rng *tau;
    int semilla=time(NULL); //Semilla del generador de números aleatorios
    tau=gsl_rng_alloc(gsl_rng_taus); //Inicializamos el puntero
    gsl_rng_set(tau,semilla); //Inicializamos la semilla
    
    //Cálculo de constantes
    M=N*N; L=N-1;
    fichO.open("exp-crit_magn.txt");
    
    //Aplicamos el algoritmo de Monte-Carlo hasta alcanzar la precisión indicada
    T=T0*1.0;
    b=0;
    k=0;
    do
    {
        b0=b;
        T=T/10;
        k++;

        //Partimos de una configuración ordenada
        for(i=0;i<N;i++)
            for(j=0;j<N;j++)
                A[i][j]=true;

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
        b=log(mag)/log(T);        

        //Mostramos los resultados en pantalla
        fichO << "Exponente crítico de la magnetización: " << b << endl;
        fichO << "Temperatura (relativa a la temperatura crítica) a la que se ha obtenido: " << T << endl;
        
    } while (abs(b-b0)>tol && k<10);

    return 0;
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