//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <complex>
//#include <iomanip>
using namespace std;

#define pi 3.14159265358979 //Valor del número pi
#define N 100 //Número de puntos en los que se discretiza el eje X
#define nC 1 //Número de ciclos que completa la función de onda en nuestra red; debe cumplir n<=N/4
#define L 0.3 //L denota a lambda (parámetro del potencial)
#define T 100 //Instante de tiempo máximo hasta el que se ejecuta el programa

//Cabecera con todas las funciones que hemos definido
void potencial(double V[], double k);
void onda_inicial(complex<double> phi0[], double k, double mu, double sigma2);

int main (void)
{
    double k,s,V[N+1],mu,sigma2;
    int i,j;
    complex<double> phi0[N+1];

    //Calculamos las constantes necesarias a partir de las fijadas
    k=2*pi*nC/N;
    s=1.0/(4*k*k);
    potencial(V,k); //Valor del potencial en cada punto de la red discretizada
    mu=N/4.0; sigma2=N*N/256.0; //Valores indicados en el guion en unidades de h
    onda_inicial(phi0,k,mu,sigma2); //Inicializamos el valor de la función de onda en cada punto

    cout << phi0[0];

    //alpha();

    //Ej

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
    phi0[0]=phi0[N]=0;
    for(j=1;j<N;j++)
    {
        mod=exp(-(j-mu)*(j-mu)/den); phase=k*j; 
        phi0[j]=polar(mod,phase);
    }

    return;
}