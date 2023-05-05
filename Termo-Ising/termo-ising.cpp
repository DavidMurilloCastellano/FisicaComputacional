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
#define ptos 100//Número de puntos a graficar
#define pMC 1e4 //Número de pasos de Monte-Carlo que se dan para calcular cada promedio de magnitudes
#define T1 1.5 //Extremo inferior del intervalo de temperaturas
#define T2 3.5 //Extremo superior del intervalo de temperaturas
#define nT 10 //Número de valores de temperatura que se van a considerar en el intervalo [T1,T2]

//Cabecera con todas las funciones que hemos definido
int b2i(bool b);
int DEnergia(bool A[][N],int n, int m);
int magn(bool A[][N]);
double energia(bool A[][N]);
int corr(bool A[][N], int i);

int main (void)
{
    double T, h, p, en, en2, sMag, sMag2, sE, sE2, sE4, mMag, mE, mE2, varMag, varE, varE2, aux;
    int i1,i2, M, L, n, m, j, k, mgn, e;
    bool A[N][N];
    ofstream fichO;

    //Para la generación de números aleatorios con GSL
    gsl_rng *tau;
    int semilla=time(NULL); //Semilla del generador de números aleatorios
    tau=gsl_rng_alloc(gsl_rng_taus); //Inicializamos el puntero
    gsl_rng_set(tau,semilla); //Inicializamos la semilla
    
    

    //Cálculo de constantes
    M=N*N; L=N-1;
    fichO.open("magnitudes-N="+to_string(N)+".txt");
    
    //El algoritmo se ejecuta para cada una de las temperaturas consideradas
    h=(T2-T1)/(nT-1); 
    T=T1;
    for(k=0; k<nT; k++)
    {
        
        //Partimos de una configuración ordenada
        for(i1=0;i2<N;i1++)
            for(i2=0;i2<N;i2++)
                A[i1][i2]=true;

        //Calculamos el valor inicial de la energía del sistema
        en=energia(A);
        fichO << T << endl;
        for(i1=1;i1<=ptos;i1++) //Tendremos ptos puntos en las gráficas
        {
            //Inicializamos las sumas
            sMag=sMag2=sE=sE2=sE4=0.0;
            for(i2=0;i2<pMC;i2++)
            {
                //Elegimos un nodo al azar y aplicamos el algoritmo durante 1 pMC
                for(j=0; j<M; j++)
                {
                    n=gsl_rng_uniform_int(tau,N); m=gsl_rng_uniform_int(tau,N);
                    e=DEnergia(A,n,m);
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
                mgn=magn(A);
                sMag=sMag+mgn; sMag2=sMag2+mgn*mgn;
                en2=en*en;
                sE=sE+en; sE2=sE2+en2; sE4=sE4+en2*en2;                
            }
            //Calculamos los promedios y las varianzas correspondientes
            mMag=sMag/pMC; mE=sE/pMC; mE2=sE2/pMC; aux=mE*mE;
            varMag=sMag2/pMC-mMag*mMag; varE=mE2-aux; varE2=sE4/pMC-mE2*mE2;
             
            //Volcamos al fichero los resultados obtenidos para representarlos luego
            //Se ha aplicado un factor de cobertura correspondiente a un nivel de confianza del 95%
            fichO << i1 << ", " << mMag/M << ", " << 1.96*sqrt(varMag/pMC)/M << ", "; //Cálculo de la magnetización promedio
            fichO << mE/(2*N) << ", " << 1.96*sqrt(varE/pMC)/(2*N) << ", "; //Cálculo de la energía media
            fichO << varE/(M*T) << ", " << 1.96*sqrt((varE2+4*aux*varE)/pMC)/(M*T) << endl;//Cálculo del calor específico   
        }
        fichO << endl;

        T=T+h;
       }

    fichO.close();

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

//Función energia: devuelve la energía del sistema en un determinado instante (multiplicada por 2 por optimización)
//Argumentos: A[][], matriz con la orientación de los espines, 
//Retorno: e, energía del sistema (por 2)
double energia(bool A[][N])
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

    return -e/2.0;
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
