//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include "gsl_rng.h" //Libreria para generación de números aleatorios
using namespace std;

#define N 256 //Número máximo de nodos del sistema en cada eje
#define pMC 5e4 //Número de pasos de Monte-Carlo que se dan para calcular cada promedio de magnitudes
#define T1 2.17 //Extremo inferior del intervalo de temperaturas
#define T2 2.39 //Extremo superior del intervalo de temperaturas
#define nT 20 //Número máximo de valores de temperatura que se van a considerar en el intervalo [T1,T2]

//Cabecera con todas las funciones que hemos definido
int b2i(bool b);
int DEnergia(bool A[][N],int n, int m, int N0);
double energia(bool A[][N], int N0);

int main (void)
{
    double T, h, p, en, en2, sE, sE2, sE4, mE, mE2, varE, varE2, aux, CEsp0, CEsp;
    int i1,i2, M, L, n, m, j, k, e, N0;
    bool A[N][N], Max;
    ofstream fichOTcN, fichOCeT;

    //Para la generación de números aleatorios con GSL
    gsl_rng *tau;
    int semilla=time(NULL); //Semilla del generador de números aleatorios
    tau=gsl_rng_alloc(gsl_rng_taus); //Inicializamos el puntero
    gsl_rng_set(tau,semilla); //Inicializamos la semilla

    //Abrimos los ficheros
    fichOTcN.open("temp-critica.txt");
    fichOCeT.open("calor_esp-temp.txt");

    //Ejecutamos el método de Monte-Carlo para distintos tamaños del sistema
    N0=16;
    do
    {
        //Cálculo de constantes
        N0=2*N0;
        M=N0*N0; L=N0-1;
        fichOCeT << N0 << endl;
        CEsp=0.0;
        
        //El algoritmo se ejecuta para cada una de las temperaturas consideradas
        h=(T2-T1)/(nT-1); 
        T=T1;
        k=0;
        Max=false;
        do
        {
            //Partimos de una configuración ordenada
            for(i1=0;i1<N0;i1++)
                for(i2=0;i2<N0;i2++)
                    A[i1][i2]=true;

            //Calculamos el valor inicial de la energía del sistema
            en=energia(A,N0);
            CEsp0=CEsp;
            //Inicializamos las sumas
            sE=sE2=sE4=0.0;

            //Ejecutamos las primeras iteraciones para que el sistema se equilibre
            for(i1=0;i1<1e4;i1++)
            {
                //Elegimos un nodo al azar y aplicamos el algoritmo durante 1 pMC
                for(j=0; j<M; j++)
                {
                    n=gsl_rng_uniform_int(tau,N0); m=gsl_rng_uniform_int(tau,N0);
                    e=DEnergia(A,n,m,N0);
                    p=exp(-e/T);
                    if(p>1 || gsl_rng_uniform(tau)<p)
                    {
                        en=en+e;
                        if(A[n][m])
                            A[n][m]=false;
                        else
                            A[n][m]=true;
                    }
                }         
            }

            //Ejecutamos el algoritmo para calcular los promedios
            for(i1=0;i1<pMC;i1++)
            {
                //Elegimos un nodo al azar y aplicamos el algoritmo durante 1 pMC
                for(j=0; j<M; j++)
                {
                    n=gsl_rng_uniform_int(tau,N0); m=gsl_rng_uniform_int(tau,N0);
                    e=DEnergia(A,n,m,N0);
                    p=exp(-e/T);
                    if(p>1 || gsl_rng_uniform(tau)<p)
                    {
                        en=en+e;
                        if(A[n][m])
                            A[n][m]=false;
                        else
                            A[n][m]=true;
                    }
                }         
                //Calculamos las sumas para obtener los promedios posteriores
                en2=en*en;
                sE=sE+en; sE2=sE2+en2; sE4=sE4+en2*en2;    
            }   

            //Calculamos los promedios y las varianzas correspondientes
            mE=sE/pMC; mE2=sE2/pMC; aux=mE*mE;
            varE=mE2-aux; varE2=sE4/pMC-mE2*mE2;
            CEsp=varE/(M*T);
            
            //Volcamos al fichero los resultados obtenidos para representarlos luego
            //Se ha aplicado un factor de cobertura correspondiente a un nivel de confianza del 95%
            fichOCeT << T << ", " << CEsp << ", " << 1.96*sqrt((varE2+4*aux*varE)/pMC)/(M*T) << endl;

            //Comprobamos si se ha alcanzado el máximo
            if (CEsp < CEsp0)
                Max=true;
            
            T=T+h;
            k++;
        }while(k<nT && !Max);

        fichOCeT << endl;
        fichOTcN << N0 << ", " << T-2*h << endl;

    } while (N0<N);
    
    fichOCeT.close();
    fichOTcN.close();

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
//Argumentos:A[][], vector de espines; n,m, coordenadas del nodo seleccionado; N0, tamaño del sistema 
//Retorno: energía asociada a la transición
int DEnergia(bool A[][N],int n, int m, int N0)
{
    int e;

    if(n==N0-1)
        e=b2i(A[0][m]);
    else
        e=b2i(A[n+1][m]);

    if(n==0)
        e=e+b2i(A[N0-1][m]);
    else
        e=e+b2i(A[n-1][m]);

    if(m==N0-1)
        e=e+b2i(A[n][0]);
    else
        e=e+b2i(A[n][m+1]);

    if(m==0)
        e=e+b2i(A[n][N0-1]);
    else
        e=e+b2i(A[n][m-1]);

    e=e*2*b2i(A[n][m]);

    return e;
}


//Función energia: devuelve la energía del sistema en un determinado instante (multiplicada por 2 por optimización)
//Argumentos: A[][], matriz con la orientación de los espines; N0, tamaño del sistema 
//Retorno: e, energía del sistema
double energia(bool A[][N], int N0)
{
    int e,i,j;

    e=b2i(A[0][0])*(b2i(A[0][1])+b2i(A[0,N0-1])+b2i(A[1][0])+b2i(A[N0-1][0])); //Energía nodo (0,0)

    for(j=1;j<N0-1;j++)
        e=e+b2i(A[0][j])*(b2i(A[0][j+1])+b2i(A[0][j-1])+b2i(A[1][j])+b2i(A[N0-1][j])); //Energía nodos (0,1),...,(0,N0-2)
    
    e=e+b2i(A[0][N0-1])*(b2i(A[0][0])+b2i(A[0][N0-2])+b2i(A[1][N0-1])+b2i(A[N0-1][N0-1])); //Energía nodo (0,N0-1)

    for(i=1;i<N0-1;i++)
    {
        e=e+b2i(A[i][0])*(b2i(A[i][1])+b2i(A[i][N0-1])+b2i(A[i+1][0])+b2i(A[i-1][0])); //Energía nodo (i,0)

        for(j=1;j<N0-1;j++)
            e=e+b2i(A[i][j])*(b2i(A[i][j+1])+b2i(A[i][j-1])+b2i(A[i+1][j])+b2i(A[i-1][j])); //Energía nodos (i,1),...,(i,N0-2)

        e=e+b2i(A[i][N0-1])*(b2i(A[i][0])+b2i(A[i][N0-2])+b2i(A[i+1][N0-1])+b2i(A[i-1][N0-1])); //Energía nodo (i,N0-1)
    }

    e=e+b2i(A[N0-1][0])*(b2i(A[N0-1][1])+b2i(A[N0-1][N0-1])+b2i(A[0][0])+b2i(A[N0-2][0])); //Energía nodo (N0-1,0)

    for(j=1;j<N0-1;j++)
        e=e+b2i(A[N0-1][j])*(b2i(A[N0-1][j+1])+b2i(A[N0-1][j-1])+b2i(A[0][j])+b2i(A[N0-2][j])); //Energía nodos (N0-1,1),...,(N0-1,N0-2)
    
    e=e+b2i(A[N0-1][N0-1])*(b2i(A[N0-1][0])+b2i(A[N0-1][N0-2])+b2i(A[0][N0-1])+b2i(A[N0-2][N0-1])); //Energía nodo (N0-1,N0-1)

    return -e/2.0;
}
