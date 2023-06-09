//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include "gsl_rng.h" //Libreria para generación de números aleatorios
//#include <iomanip>
using namespace std;

#define Pi 3.14159265358979 //Valor del número Pi
#define N 16 //Número de átomos que conforman nuestro sistema
#define L 10 //Tamaño (en las unidades consideradas) de cada lado de la caja cuadrada
#define nT 1 //Cantidad de distintos valores de temperatura que se estudiarán
#define T1 20 //Instante inicial de tiempo para estudiar las funciones de interés
#define T2 50 //Instante final de tiempo para estudiar las funciones de interés
#define K 1 //Factor en el que se incrementa la velocidad de las partículas al calentar el sistema
#define v0 2 //Módulo de la velocidad inicial de las partículas en la caja
#define s 1e-4 //Paso con el que se aplica el algoritmo
#define S 50 //Límite de tiempo hasta el que se considera la simulación
#define D 200 //Número de líneas que se pasan al fichero para crear el vídeo de la simulación
#define R 1 //Separación mínimia inicial entre cada par de partículas
#define B 120 //Número de bins en el que se divide el eje de abscisas en la función de correlación
#define E true //Indica si se desea calcular o no la energía del sistema
#define Desp false //Indica si se quiere calcular el desplazamiento de una partícula respecto su posición inicial
#define Sep false //Indica si se quiere calcular la distancia entre dos partículas

//Cabecera con todas las funciones que hemos definido
void cond_inic_aleatorio(int n,gsl_rng *tau);
void cond_inic_vx(int n, gsl_rng *tau);
void cond_inic_unif(int n,gsl_rng *tau);
void cond_inic_panal(int n);
double dist(double x[], double y[]);
bool ac(double a[][2], double r[][2], int n, double& V);
double rh(double r[][2], double v[][2], double a[][2], double h, int n);
void vh(double v[][2], double a[][2], double h, int n);
double temp(float a, float b,int n);
void HistVel(float a, float b, int n, double vMx, double vMy, double vMm, double T);
void corr(void);


int main(void)
{
    int i,j,k,l,t0[10],m;
    double a[N][2], r[N][2], v[N][2], v2, h, T, V, t, vMx, vMy, vMm, P, r0[2], r1[2], desp, d, x, y, sep;
    ifstream fichIn;
    ofstream fichOPos, fichOVel, fichOE, fichOT, fichOPT, fichODesp, fichOSep;
    bool div;

    gsl_rng *tau;
    int semilla=time(NULL); //Semilla del generador de números aleatorios
    tau=gsl_rng_alloc(gsl_rng_taus); //Inicializamos el puntero
    gsl_rng_set(tau,semilla); //Inicializamos la semilla
    
    //Generamos aleatoriamente las condiciones iniciales
    //cond_inic_aleatorio(N, tau);
    //cond_inic_vx(N,tau);
    cond_inic_unif(N,tau);
    //cond_inic_panal(N);

    //Copiamos las velocidades y posiciones iniciales aleatorias
    fichIn.open("pos-vel_iniciales.txt");
    i=0;
    while(!fichIn.eof() && i<N)
    {
        //if(!fichIn.is_open())
            //cout << "Error al abrir el fichero";

        fichIn >> r[i][0];
        fichIn >> r[i][1];
        fichIn >> v[i][0];
        fichIn >> v[i][1];
        i++;
    }
    fichIn.close();

    fichOPos.open("particulas-v0="+to_string(v0)+"_posiciones.txt");
    fichOVel.open("particulas-v0="+to_string(v0)+"_velocidades.txt");
    fichOE.open("particulas-v0="+to_string(v0)+"_energias.txt");
    fichOT.open("particulas-v0="+to_string(v0)+"_temperatura.txt");
    fichOPT.open("particulas-v0="+to_string(v0)+"_P-T.txt");
    fichODesp.open("particulas_desplazamiento.txt");
    fichOSep.open("particulas_separacion.txt");

    
    //Comprobamos que el número de partículas coincide con la cantidad de datos leídos
    if(i!=N)
        fichOPos << "El número de partículas indicado no coincide con la cantidad de datos iniciales." << endl;
    else
    {
        l=1; k=1;
        h=s;
        fichOPos.precision(6); 
        fichOVel.precision(6);
        T=V=P=0.0;
        vMx=vMy=vMm=0.0;
        //Escribimos un vector con los instantes de tiempo en los que calentamos el sistema
        t0[0]=T1; //Esperamos a que el sistema alcance el equilibrio
        t0[1]=60;
        for(i=2;i<nT;i++)
            t0[i]=t0[i-1]+60;
        //t0[2]=30; t0[3]=35; t0[4]=45;
        t0[nT]=S;
        if(nT>9)
            fichOT << "Aumentar la dimensión del vector de temperaturas para calentar" << endl;

        //Volcamos las posiciones y velocidades iniciales
        fichOVel << "0.00" << endl;
        for(i=0;i<N;i++)
        {
            v2=v[i][0]*v[i][0]+v[i][1]*v[i][1];

            //Calculamos la energía cinética inicial
            if(E)
                T=T+v2/2;

            v2=sqrt(v2);
            fichOPos << r[i][0] << " " << r[i][1] << endl;
            fichOVel << v[i][0] << " " << v[i][1] << " " << v2 << endl;
            //Calculamos los valores máximos de velocidad para hacer el histograma luego
            if(vMx<abs(v[i][0]))
                vMx=abs(v[i][0]);
            if(vMy<abs(v[i][1]))
                vMy=abs(v[i][1]);
            if(vMm<v2)
                vMm=v2;
        }
        fichOPos << endl;
        fichOE << "0.00 " << T << " ";

        //Calculamos las aceleraciones (y energía potencial) iniciales
        div=ac(a,r,N,V);
        if(E)
            fichOE << V << " " << T+V <<endl;
        //Copiamos la posición inicial de la primera partícula
        if(Desp)
        {
            r0[0]=r[1][0]; r0[1]=r[1][1];
            x=r0[0]; y=r0[1];
            desp=0.0;
        }
        
        if(Sep)
        {
            sep=0.0;
            fichOSep << "0, 1" << endl;
        }
        
        while (l<=nT)
        {
            m=0;
            //Calentamos el sistema aumentando la velocidad de las partículas
            if(l>1)
            {
                for(i=0; i<N; i++)
                {
                    v[i][0]=K*v[i][0];
                    v[i][1]=K*v[i][1];
                }
            }

            //Aplicamos el algoritmo de Verlet
            while(h<t0[l] && div)
            {
                if(h>=T1 && h<=T2)
                {
                    P=P+rh(r,v,a,s,N);
                    m++;
                }
                else
                    rh(r,v,a,s,N);

                //Calculamos el desplazamiento de la primera partícula respecto a su posición inicial
                if(Desp)
                {
                    r1[0]=r[1][0]; r1[1]=r[1][1];
                    d=dist(r1,r0);
                    desp=desp+d*d;
                    r0[0]=x; r0[1]=y;
                }

                //Calculamos la separación cuadrática entre las dos primeras partículas
                if(Sep)
                {
                    r0[0]=r[1][0]; r0[1]=r[1][1];
                    r1[0]=r[2][0]; r1[1]=r[2][1];

                    d=dist(r0,r1);
                    sep=sep+d*d;
                }
                
                vh(v,a,s,N);
                div=ac(a,r,N,V);
                vh(v,a,s,N);

                if(k%D==0) //No pasamos todas las posiciones a la simulación
                {
                    fichOVel << h << endl;
                    T=0.0;
                    for(i=0;i<N;i++)
                    {
                        v2=v[i][0]*v[i][0]+v[i][1]*v[i][1];
                        if(E)
                            T=T+v2/2;
                        v2=sqrt(v2);
                        fichOPos << r[i][0] << " " << r[i][1] << endl;
                        fichOVel << v[i][0] << " " << v[i][1] << " " << v2 << endl;
                        if(vMx<abs(v[i][0]))
                            vMx=abs(v[i][0]);
                        if(vMy<abs(v[i][1]))
                            vMy=abs(v[i][1]);
                        if(vMm<v2)
                            vMm=v2;
                    }
                    fichOPos << endl;
                    if(E)
                        fichOE << h << " " << T << " " << V << " " << T+V << endl;

                    //Calculamos el desplazamiento cuadrático medio de la primera partícula respecto su posición de equilibrio
                    if(Desp)
                        fichODesp << h << ", " << desp/D << endl;
                    desp=0.0;

                    //Calculamos la separación cuadrática media entre las dos primeras partículas
                    if(Sep && h<T2)
                        fichOSep << h << ", " << sep/D << endl;
                    sep=0.0;
                }

                h=h+s; k++;

                if (!div) //Comprobamos que no ha habido errores en los cálculos
                fichOPos << "Error al calcular la aceleración" << endl;
            }    

            //Calculamos la temperatura y la presión en el intervalo especificado en el guion
            t=temp(t0[l-1],t0[l],N);
            //P=P/(4*L*m*s); //La presión es el cambio de momento total por unidad de tiempo y área (4L al estar en dos dimensiones)

            //Pasamos a un fichero la información obtenida
            if(l+1<=nT)
                fichOT << t << ", ";
            else
                fichOT << t << endl;
            fichOPT << t << ", " << P << endl;

            //Histograma de velocidades
            //HistVel(t0[l-1],t0[l],N,vMx,vMy,vMm,t);
            l++;

            //Separamos entre los distintos valores de temperatura
            if(Sep)
                fichOSep << endl;

            vMx=vMy=vMm=P=0.0;
        } 

        corr();

        fichOPos.close();
        fichOVel.close();
        fichOE.close();
        fichOT.close();
        fichOPT.close();
        fichODesp.close();
        fichOSep.close();
    }

    return 0;
}


//Función cond_inic_aleatorio: genera de forma aleatoria las posiciones y velocidades iniciales de las partículas en la
//caja. Escribe dicha información en el fichero "pos-vel_iniciales.txt" según: en cada línea se escribe posición en
//X, posición en Y, velocidad en X, velocidad en Y
//Argumentos: n, número de partículas
void cond_inic_aleatorio(int n,gsl_rng *tau)
{
    int i,j,k;
    double v,r[N][2],r1[2],r2[2];
    ofstream fichOut;

    fichOut.open("pos-vel_iniciales.txt");
    fichOut.precision(8);

    //Generamos las posiciones y velocidades aleatorias
    for(i=0;i<n;i++)
    {
        r1[0]=gsl_rng_uniform(tau)*L; r1[1]=gsl_rng_uniform(tau)*L;
        r[i][0]=r1[0]; r[i][1]=r1[1];
        //Comprobamos que las partículas no parten de posiciones demasiado próximas
        k=0; //Para que no genere infinitos números aleatorios
        for(j=0;j<i;j++)
        {
            r2[0]=r[j][0]; r2[1]=r[j][1];
            while(dist(r1,r2)<R && k<1000)
            {
                r1[0]=gsl_rng_uniform(tau)*L; r1[1]=gsl_rng_uniform(tau)*L;
                r[i][0]=r1[0]; r[i][1]=r1[1];
                k++;
                j=0;
            }
        }
        
        if(k==1000)
        {
            fichOut << "No se generaron partículas suficientemente separadas" << endl;
            i=n;
        }

        else
        {
            v=2*Pi*gsl_rng_uniform(tau);
            fichOut << r[i][0] << " " << r[i][1] << " " << v0*cos(v) << " " << v0*sin(v) << endl; 
        }   
        
    }

    fichOut.close();    

    return;
}

//Función cond_inic_vx: genera unas condiciones iniciales para el sistema con las partículas aleatoriamente
//distribuidas por la caja pero todas con ella moviéndose hacia la derecha con una velocidad entre 0 y v0
//Argumentos:n, número de partículas del sistema
void cond_inic_vx(int n, gsl_rng *tau)
{
    int i,j,k;
    double r[N][2],r1[2],r2[2];
    ofstream fichOut;

    fichOut.open("pos-vel_iniciales.txt");
    fichOut.precision(8);

    //Generamos las posiciones y velocidades aleatorias
    for(i=0;i<n;i++)
    {
        r1[0]=gsl_rng_uniform(tau)*L; r1[1]=gsl_rng_uniform(tau)*L;
        r[i][0]=r1[0]; r[i][1]=r1[1];
        //Comprobamos que las partículas no parten de posiciones demasiado próximas
        k=0; //Para que no genere infinitos números aleatorios
        for(j=0;j<i;j++)
        {
            r2[0]=r[j][0]; r2[1]=r[j][1];
            while(dist(r1,r2)<R && k<1000)
            {
                r1[0]=gsl_rng_uniform(tau)*L; r1[1]=gsl_rng_uniform(tau)*L;
                r[i][0]=r1[0]; r[i][1]=r1[1];
                k++;
                j=0;
            }
        }
        
        if(k==1000)
        {
            fichOut << "No se generaron partículas suficientemente separadas" << endl;
            i=n;
        }

        else
            fichOut << r[i][0] << " " << r[i][1] << " " << v0*gsl_rng_uniform(tau) << " " << 0 << endl; 
        
    }

    fichOut.close();    

    return;
}

//Función cond_inic_unif: genera unas condiciones iniciales para las partículas de forma que estas se distribuyen
//de manera uniforme por la caja y tienen una velocidad inicial de módulo v0 y dirección aleatoria.
//Argumentos: n, número de partículas
void cond_inic_unif(int n,gsl_rng *tau)
{
    int i,j,m;
    double l,v;
    ofstream fichOut;

    fichOut.open("pos-vel_iniciales.txt");
    fichOut.precision(8);

    m=floor(sqrt(n)); //Número de partículas en cada línea horizontal
    l=(L*1.0)/m; //Distancia entre partículas
    //Distribuimos las partículas uniformemente por la caja y con velocidades aleatorias
    for(i=0;i<m;i++)
    {
        for(j=0;j<m;j++)
        {
            v=2*Pi*gsl_rng_uniform(tau);
            fichOut << 0.5+i*l << " " << 0.5+j*l << " " << v0*cos(v) << " " << v0*sin(v) << endl;
        }
    }

    return;
}

//Función cond_inic_panal: inicia las posiciones de las partículas en la caja (rectangular de altura L) y en reposo.
//Escribe dicha información en el fichero "pos-vel_iniciales.txt" según: en cada línea se escribe posición en
//X, posición en Y, velocidad en X, velocidad en Y
//Argumentos: n, número de partículas
void cond_inic_panal(int n)
{
    int j, i;
    double l, a, b, x1, x2, y;
    ofstream fichOut;

    l=L/12.0622; //Punto medio entre 12 y 7sqrt(3)
    a=l*sqrt(3)/2; 
    b=l/2;
    fichOut.open("pos-vel_iniciales.txt");

    for(i=0;i<7; i++)
    {
        x1=a*(2*i+1); x2=2*a*i;
        for(j=0;j<4;j++)
        {
            y=3*j*l;

            fichOut << x1 << " " << y+0.0 << " " << 0.0 << " " << 0.0 << endl;
            fichOut << x2 << " " << y+b << " " << 0.0 << " " << 0.0 << endl;
            fichOut << x2 << " " << y+b+l << " " << 0.0 << " " << 0.0 << endl;
            fichOut << x1 << " " << y+2*l << " " << 0.0 << " " << 0.0 << endl;
        }
    }

    fichOut.close();   
    return;
}

//Función dist: calcula la distancia entre dos puntos de la caja teniendo en cuenta las condiciones de contorno
//periódicas.
//Argumentos: x, y; vectores de 2-dim que contienen las coordenadas de cada punto.
//Retorno: d, distancia entre los puntos dados
double dist(double x[], double y[])
{
    double d,a,b;

    a=abs(x[0]-y[0]); b=abs(x[1]-y[1]);
    if(a>L/2)
    {
        a=L-a;
        if(x[0]<y[0])
            x[0]=x[0]+L;
        else
            y[0]=y[0]+L;
    }

    if(b>L/2)
    {
        b=L-b;
        if(x[1]<y[1])
            x[1]=x[1]+L;
        else
            y[1]=y[1]+L;
    }
        
    d=sqrt(a*a+b*b);

    return d;
}

//Función ac: calcula la aceleración a la que está sometida cada partícula en un instante determinado a partir
//del potencial de Lennard-Jones. Para optimizar, calcula también la energía potencial del sistema
//Argumentos: a,r; matrices de aceleración y posición de cada partícula en el instante considerado, respect.;
//n, número de partículas del sistema; V; energía potencial del sistema.
//Retorno: verdadero si la distancia (denominador) no es nulo, falso en caso contrario
bool ac(double a[][2], double r[][2], int n, double& V)
{
    double f[N], d, d6, d7, r1[2], r2[2], f0, f1;
    int i,j;
    bool div;

    f[0]=0.0; //Un cuerpo no ejerce fuerza sobre sí mismo
    div=true;
    V=0.0;
    for(i=0;i<n;i++) 
        a[i][0]=a[i][1]=0.0; //Inicializamos las sumas a 0

    for(i=0;i<n;i++)
    {
        for(j=i+1;j<n;j++)
        {
            r1[0]=r[i][0]; r1[1]=r[i][1]; //Para volver a tomar la posición real de la partícula que puede haberse
            r2[0]=r[j][0]; r2[1]=r[j][1]; //modificado por la función dist
            d=dist(r1,r2);
            if(d<1e-18) //Comprobamos que no dividimos entre 0
            {
                a[i][0]=a[i][1]=0.0;
                j=n;
                div=false;
            }
            else if(d<3) //A distancias largas consideramos la fuerza nula
            {
                d6=pow(d,6); d7=d6*d;
                f[j]=24/d7*(2/d7-1/d); //Calculamos el módulo de la fuerza en las unidades dadas
                f0=f[j]*(r2[0]-r1[0]); //Calculamos el valor de cada componente
                f1=f[j]*(r2[1]-r1[1]);

                //Calculamos la aceleración proporcionada a la partícula j por la fuerza creada por i
                a[j][0]=a[j][0]+f0;
                a[j][1]=a[j][1]+f1;

                //Aplicamos la 3ª ley de Newton para calcular la fuerza experimentada por la partícula i
                a[i][0]=a[i][0]-f0;
                a[i][1]=a[i][1]-f1; 

                //Calculamos la energía potencial
                if(E)
                    V=V+4/d6*(1/d6-1);
            }
        }
    }

    return div;
}

//Función rh: calcula las posiciones en un instante h posterior a partir del desarrollo en serie
//Argumentos: r, v, a, matrices posiciones, velocidades y aceleraciones en el instante anterior;
//h, paso; n, número de cuerpos
double rh(double r[][2], double v[][2], double a[][2], double h, int n)
{
    int i,k;
    double d, p;

    d=h*h/2; //Calculamos las constantes fuera del bucle por optimización
    p=0.0;
    for (i=0;i<n;i++)
    {
        r[i][0]=r[i][0]+h*v[i][0]+d*a[i][0];
        //Comprobamos que las partículas no chocan con el borde de la caja
        if(r[i][0]<0)
        {
            r[i][0]=r[i][0]+L;
            p=p+2*abs(v[i][0]);
        }
        if(r[i][0]>L)
        {
            r[i][0]=r[i][0]-L;
            p=p+2*abs(v[i][0]);
        }

        r[i][1]=r[i][1]+h*v[i][1]+d*a[i][1];
        //Comprobamos que las partículas no chocan con el borde de la caja
        if(r[i][1]<0)
        {
            r[i][1]=r[i][1]+L;r[i][1]=r[i][1]+L;
            p=p+2*abs(v[i][1]);
        }
        if(r[i][1]>L)
        {
            r[i][1]=r[i][1]-L;
            p=p+2*abs(v[i][1]);
        }
            
    }
    return p;
}

//Función vh: calcula los dos primeros sumandos de la velocidad en un instante h posterior a partir del desarrollo 
//en serie
//Argumentos: v, a, matrices posiciones, velocidades y aceleraciones en el instante anterior;
//h, paso; n, número de cuerpos
void vh(double v[][2], double a[][2], double h, int n)
{
    int i;
    double d;

    d=h/2;
    for (i=0;i<n;i++)
    {
        v[i][0]=v[i][0]+d*a[i][0];
        v[i][1]=v[i][1]+d*a[i][1];
    }

    return;
}

//Función temp: calcula la temperatura media del sistema en un intervalo de tiempo. Para ello, lee los datos de
//energía cinética del sistema del fichero "particulas_energias.txt".
//Argumentos: a,b, extremos del intervalo en el que se calcula la temperatura; n, número de partículas del sistema; 
double temp(float a, float b,int n)
{
    double t,aux,h;
    int i;
    ifstream fichIn;

    t=0.0;
    fichIn.open("particulas-v0="+to_string(v0)+"_energias.txt");
    

    if(a>=0 && b<=S && a<b) //Se comprueba que los límites están dentro del rango de tiempo calculado
    {
        if(!fichIn.is_open())
            t=0.0;
        else
        {
            fichIn >> h;
            while(h<a)
                for(i=0;i<4;i++) //Descartamos los datos que no nos interesan
                    fichIn >> h;
            while(h<=b && !fichIn.eof())
            {
                fichIn >> aux;
                for(i=0;i<3;i++) //Descartamos los datos que no nos interesan
                    fichIn >> h;
                t=t+aux;
            }
            t=t/(n*(floor((b-a)/(s*D))+1)); //Promediamos en el número de partículas y tiempo (iteraciones calculadas
                                            //en [a,b]) 
        }
    }
    
    fichIn.close();

    return t;
}

//Función HistVel: pasa a un fichero la distribución relativa de las partículas según el promedio, en el instante
//de tiempo especificado, de la velocidad que llevan en cada componente y el módulo.
//Argumentos: a,b, límites del intervalo donde se calcula el promedio; n, número de partículas; T, temperatura del
//sistema
void HistVel(float a, float b, int n, double vMx, double vMy, double vMm, double T)
{
    float t, aux, vx[N], vy[N], vm[N], lx, ly, lm;
    int i, j, M, m;
    ifstream fichIn;
    ofstream fichOut;

    //Iniciamos los datos
    t=0.0;
    for(i=0;i<n;i++)
    {
        vx[i]=vy[i]=vm[i]=0.0;
    }
    fichIn.open("particulas-v0="+to_string(v0)+"_velocidades.txt");
    if(nT==1)
        fichOut.open("histograma-velocidades-v0="+to_string(v0)+".txt");
    else
        fichOut.open("histograma-velocidades-v0="+to_string(v0)+"_temp-T="+to_string(T)+".txt");

    if(a>=0 && b<=S && a<b) //Se comprueba que los límites están dentro del rango de tiempo calculado
    {
        if(!fichIn.is_open())
            fichOut << "No se pudo abrir el archivo";
        else
        {
            m=0;
            //Calculamos la longitud de cada subintervalo
            lx=2*vMx/n; ly=2*vMy/n; //Hay valores positivos y negativos
            lm=vMm/n;
            fichIn >> t;
            while(t<a)
            {
                for(i=0;i<n;i++) //Descartamos los datos que no nos interesan
                    for(j=0;j<2;j++)
                        fichIn >> t;
                fichIn >> t;
            }
            
            while(t<=b && !fichIn.eof())
            {
                m++;
                for(i=0;i<n; i++)
                {
                    //Eje X
                    fichIn >> aux;
                    aux=aux+vMx;
                    j=floor(aux/lx);
                    if(j>=n)
                        j=n-1;
                    if(j<0)
                        j=0;
                    vx[j]++;

                    //Eje Y
                    fichIn >> aux;
                    aux=aux+vMy;
                    j=floor(aux/ly);
                    if(j>=n)
                        j=n-1;
                    if(j<0)
                        j=0;
                    vy[j]++;

                    //Módulo
                    fichIn >> aux;
                    j=floor(aux/lm);
                    if(j>=n)
                    j=n-1;
                    vm[j]++;
                }

                if(!fichIn.eof())
                    fichIn >> t;             
            }

            //Calculamos el promedio de partículas con velocidad en cada subintervalo
            //M=n*(floor((b-a)/(s*D))+1);
            M=n*m;
            //Aproximamos la integral por el producto de la función de distribución por la amplitud del intervalo
            for(i=0;i<n;i++)
            {
                vx[i]=vx[i]/(lx*M);
                vy[i]=vy[i]/(ly*M);
                vm[i]=vm[i]/(lm*M);
            }

            //Pasamos al fichero la información obtenida
            for(i=0;i<n;i++)
                fichOut << lx*(i+0.5)-vMx << ", " << vx[i] << endl;
            fichOut << endl;
            for(i=0;i<n;i++)
                fichOut << ly*(i+0.5)-vMy << ", " << vy[i] << endl;
            fichOut << endl;
            for(i=0;i<n;i++)
                fichOut << lm*(i+0.5) << ", " << vm[i] << endl;
            fichOut << endl;
        }
    }
    fichIn.close();
    fichOut.close();

    return;
}

//Función corr: devuelve la función de correlación de pares centrada en la primera partícula del sistema. Para ello,
//lee las posiciones de cada partícula del archivo "particulas_posiciones.txt" y los pasa a "particulas_corr.txt",
//cambiando el origen del sistema de referencia a la posición de dicha partícula.
void corr(void)
{
    double x, y, r0[2], r1[2], h, d, c;
    int i, j, k, b[B];
    ifstream fichIn;
    ofstream fichOut;

    h=(L/sqrt(2)-1)/(B*1.0); //Anchura de cada bin
    c=T1/(s*D)+1;
    for(i=0;i<B;i++)
        b[i]=0;

    fichIn.open("particulas-v0="+to_string(v0)+"_posiciones.txt");
    fichOut.open("particulas_corr.txt");

    if(!fichIn.is_open())
        fichOut << "No se pudo abrir el archivo" << endl;
    else
    {
        k=0;
        while (!fichIn.eof())
        {
            k++;
            fichIn >> r0[0];
            fichIn >> r0[1];

            x=r0[0]; y=r0[1];

            for(i=1;i<N;i++)
            {
                fichIn >> r1[0];
                fichIn >> r1[1];

                if(k>c)
                {
                    d=dist(r1,r0);
                    r0[0]=x; r0[1]=y;

                    if(d>1 && d<7) //Comprobamos que la distancia esté en el intervalo que nos piden estudiar
                    {
                        j=floor((d-1)/h); //Quitamos el extremo inferior del intervalo
                        b[j]++;
                    }        
                }
            }
        }

        for(i=0;i<B;i++)
            fichOut << 1+(i+0.5)*h << ", " << (b[i]*1.0)/(h*(N-1)*(k-c)) << endl;
    }

    fichIn.close();
    fichOut.close();

    return;
}