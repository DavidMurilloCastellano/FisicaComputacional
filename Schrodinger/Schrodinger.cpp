//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <complex>
//#include <iomanip>
using namespace std;

#define pi 3.14159265358979 //Valor del número pi
#define N 100 //Número de puntos en los que se discretiza el eje X
#define nC 10 //Número de ciclos que completa la función de onda en nuestra red; debe cumplir n<=N/4
#define L 0.3 //L denota a lambda (parámetro del potencial)
#define T 200 //Instante de tiempo máximo hasta el que se ejecuta el programa

//Cabecera con todas las funciones que hemos definido
void potencial(double V[], double k);
void onda_inicial(complex<double> phi0[], double k, double mu, double sigma2);
void inic_alpha(complex<double> a[], double s, double V[]);
void inic_beta(complex<double> b[], double s, double V[], complex<double> phi[], complex<double> a[]);
void chi(complex<double> x[],complex<double> a[],complex<double> b[]);
void funcion_onda (complex<double> x[], complex<double> phi0[], complex<double> phi[]);
void nuevo_ciclo(complex<double> phi0[], complex<double> phi[]);

int main (void)
{
    double k,s,V[N+1],mu,sigma2,aux,sum, norma;
    int i,j,n,l;
    complex<double> phi0[N+1], a[N], b[N], x[N+1], phi[N+1];
    ofstream fichODat, fichONorm;

    //Calculamos las constantes necesarias a partir de las fijadas
    fichODat.open("schrodinger_data.dat");
    fichONorm.open("norma.txt");
    k=2*pi*nC/N;
    s=1.0/(8*k*k); //Valor del guion: s=1.0/(4*k*k);
    potencial(V,k); //Valor del potencial en cada punto de la red discretizada
    mu=N/4.0; sigma2=N*N/256.0; //Valores indicados en el guion en unidades de h
    onda_inicial(phi0,k,mu,sigma2); //Inicializamos el valor de la función de onda en cada punto
    inic_alpha(a,s,V); //Calculamos los parámetros alpha

    //Representamos la norma en cada nodo del eje y calculamos la norma total integrando por el método del trapecio
    aux=norm(phi0[0]);
    sum=aux;
    fichODat << 0 << ", " << aux << ", " << V[0] << endl;
    for(l=1;l<N;l++)
    {
        aux=norm(phi0[l]);
        sum=sum+2*aux;
        fichODat << l << ", " << aux << ", " << V[l] << endl;
    }
    aux=norm(phi0[N]);
    sum=sum+aux;
    fichODat << N << ", " << aux << ", " << V[N] << endl;
        
    fichODat << endl;
    norma=sum/2; //Copiamos el valor de la integral en el instante inicial para normalizar
    fichONorm << 0 << ", " << 1 << endl;

    //Comenzamos con el bucle temporal
    for(n=1;n<=T;n++)
    {
        inic_beta(b,s,V,phi0,a); //Calculamos los parámetros beta
        chi(x,a,b); //Calculamos los parámetros chi
        funcion_onda(x,phi0,phi); //Calculamos el nuevo valor de la función de onda en cada punto
        nuevo_ciclo(phi0,phi); //Copiamos el nuevo vector en el del instante previo

        //Representamos la norma en cada nodo del eje y calculamos la norma total (normalizada)
        //Integramos por el método del trapecio
        aux=norm(phi0[0]);
        sum=aux;
        fichODat << 0 << ", " << aux << ", " << V[0] << endl;
        for(l=1;l<N;l++)
        {
            aux=norm(phi0[l]);
            sum=sum+2*aux;
            fichODat << l << ", " << aux << ", " << V[l] << endl;
        }
        aux=norm(phi0[N]);
        sum=sum+aux;
        fichODat << N << ", " << aux << ", " << V[N] << endl;
            
        fichODat << endl;
        fichONorm << n << ", " << sum/(2*norma) << endl;
    }
    fichODat.close();
    fichONorm.close();

    return 0;
}


//Función potencial: guarda en un vector el valor del potencial en cada punto de la red
//Argumentos: V[], vector donde se guardan los resultados; k, nº de onda en las unidades indicadas
void potencial(double V[], double k)
{
    int i,j;
    double aux, v;

    //Potencial nulo a la izquierda de la caja
    aux=2.0*N/5;
    for(i=0;i<aux;i++)
        V[i]=0.0;

    //Potencial dado entre 2N/5 y 3N/5
    aux=3.0*N/5; v=L*k*k;
    for(j=i;j<=aux;j++)
        V[j]=v;

    //Potencial nulo a la derecha de la caja
    for(i=j;i<=N;i++)
        V[i]=0.0;
    
    return;
}

//Función onda_inicial: genera el valor de la función de onda gaussiana en cada punto de la red
//Argumentos: phi0, vector con el valor de la función de onda en cada punto; k, nº de onda en las unidades
//indicadas; mu, media de la distribución; sigma, varianza de la distribución
void onda_inicial(complex<double> phi0[], double k, double mu, double sigma2)
{
    double den,mod,phase;
    int j;

    den=2*sigma2;
    phi0[0]=phi0[N]=0; //Imponemos las condiciones de contorno
    for(j=1;j<N;j++)
    {
        mod=exp(-(j-mu)*(j-mu)/den); phase=k*j; 
        phi0[j]=polar(mod,phase);
    }

    return;
}

//Función inic_alpha: calcula los valores de los parámetros alpha necesarios para los cálculos
//Argumentos: a[], vector donde se guardan los coeficientes; s, parámetro normalizado de discretización del
//tiempo; V, vector con el valor del potencial en cada punto
void inic_alpha(complex<double> a[], double s, double V[])
{
    int j;

    a[N-1]=0.0;
    for(j=N-1;j>=1;j--)
        a[j-1]=-1.0/(-2.+2/s*1i-V[j]+a[j]);

    return;
}

//Función inic_beta: calcula los valores de los parámetros beta necesarios para los cálculos
//Argumentos: b[], vector donde se guardan los coeficientes; s, parámetro normalizado de discretización del
//tiempo; V, vector con el valor del potencial en cada punto; phi[], vector con los valores de la función de onda
//en cada punto; a[], vector con los parámetros alpha
void inic_beta(complex<double> b[], double s, double V[], complex<double> phi[], complex<double> a[])
{
    int j;

    b[N-1]=0.0;
    for(j=N-1;j>=1;j--)
        b[j-1]=(4/s*phi[j]*1i-b[j])/(-2.+2/s*1i-V[j]+a[j]);

    return;
}

//Función chi: calcula el valor de los parámetros chi en cada punto
//Argumentos: a[], b[], vectores con los parámetros alpha y beta; x[], vector donde se guardan los resultados
void chi(complex<double> x[],complex<double> a[],complex<double> b[])
{
    int j,n;

    x[0]=x[N]=0.0;
    n=N-2;
    for(j=0;j<=n;j++)
        x[j+1]=a[j]*x[j]+b[j];

    return;
}

//Función funcion_onda: calcula la función de onda a partir de la recurrencia indicada en el guion
//Argumentos: x[], vector con las componentes de chi; phi0[], función de onda del instante previo;
//phi[], nueva función de onda
void funcion_onda (complex<double> x[], complex<double> phi0[], complex<double> phi[])
{
    int j;

    for(j=0;j<=N;j++)
        phi[j]=x[j]-phi0[j];

    return;
}

//Función nuevo_ciclo: copia el valor de la nueva función de onda en el vector previo
//Argumentos: phi0[], función de onda instante previo; phi[], función de onda nuevo instante
void nuevo_ciclo(complex<double> phi0[], complex<double> phi[])
{
    int j;

    for(j=0;j<=N;j++)
        phi0[j]=phi[j];

    return;
}