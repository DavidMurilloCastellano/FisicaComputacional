//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include "gsl_rng.h" //Libreria para generación de números aleatorios
using namespace std;

#define N 16 //Número de nodos del sistema en cada eje
#define P 16 //Número de puntos que se grafican
#define pMC 1e4 //Número de pasos de Monte-Carlo que se dan para calcular cada promedio de magnitudes
#define T1 1.5 //Extremo inferior del intervalo de temperaturas
#define T2 3.5 //Extremo superior del intervalo de temperaturas
#define nT 10 //Número de valores de temperatura que se van a considerar en el intervalo [T1,T2]

//Cabecera con todas las funciones que hemos definido
int b2i(bool b);
int DEnergia(bool A[][N],int n, int m);
int corr(bool A[][N], int i);

int main (void)
{
    int M,L, k, i, j1, j2, n,m, e,H, c;
    long int sC[P], sC2[P];
    double h, T, p, mC[P], varC[P];
    bool A[N][N];
    ofstream fichO, fichOf;

    //Para la generación de números aleatorios con GSL
    gsl_rng *tau;
    int semilla=time(NULL); //Semilla del generador de números aleatorios
    tau=gsl_rng_alloc(gsl_rng_taus); //Inicializamos el puntero
    gsl_rng_set(tau,semilla); //Inicializamos la semilla

    //Cálculo de constantes
    M=N*N; L=N-1;
    fichO.open("correlacion-N="+to_string(N)+".txt");
    fichOf.open("fit_correlacion-N="+to_string(N)+".txt");

    //El algoritmo se ejecuta para cada una de las temperaturas consideradas
    h=(T2-T1)/(nT-1); 
    T=T1;
    H=N/P;
    for(k=0; k<nT; k++)
    {
        
        //Partimos de una configuración ordenada
        for(j1=0;j1<N;j1++)
            for(j2=0;j2<N;j2++)
                A[j1][j2]=true;

        //Inicializamos la suma
        for(j1=0;j1<P;j1++)
        {
            sC[j1]=0;
            sC2[j1]=0; 
        }
        
        for(j1=0;j1<pMC;j1++)
        {
            //Elegimos un nodo al azar y aplicamos el algoritmo durante 1 pMC
            for(j2=0; j2<M; j2++)
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

            //Calculamos la función de correlación en determinados nodos y sumamos
            for(j2=0;j2<P;j2++)
            {
                c=corr(A,j2*H);
                sC[j2]=sC[j2]+c;
                sC2[j2]=sC2[j2]+c*c;
            }
        }
        //Promediamos en cada nodo y pasamos a un fichero
        for(j2=0;j2<P;j2++)
        {
            i=j2*H;
            mC[j2]=sC[j2]*1.0/pMC;
            varC[j2]=sC2[j2]*1.0/pMC-mC[j2]*mC[j2];
            mC[j2]=mC[j2]/M;
            varC[j2]=sqrt(varC[j2]/pMC)/M;

            fichO << i << ", " << mC[j2] << ", " << 1.96*varC[j2] << endl;
            fichOf << i << ", " << log(i*mC[j2]) << ", " << 1.96*varC[j2]/mC[j2] << endl;
        }   
        T=T+h;
    }

    fichO.close();
    fichOf.close();

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