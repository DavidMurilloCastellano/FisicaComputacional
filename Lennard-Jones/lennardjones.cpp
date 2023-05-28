//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
//#include <iomanip>
using namespace std;

#define Pi 3.14159265358979 //Valor del número Pi
#define N 16 //Número de átomos que conforman nuestro sistema
#define L 4 //Tamaño (en las unidades consideradas) de cada lado de la caja cuadrada
#define s 2e-3 //Paso con el que se aplica el algoritmo
#define S 1e-4 //Límite de tiempo hasta el que se considera la simulación
#define D 2 //Número de líneas que se pasan al fichero para crear el vídeo de la simulación
#define R 0.1 //Separación mínimia inicial entre cada par de partículas
#define E false //Indica si se desea calcular o no la energía del sistema

//Cabecera con todas las funciones que hemos definido
void cond_inic_aleatorio(int n);
void cond_inic_panal(int n);
double dist(double x[], double y[]);
bool ac(double a[][2], double r[][2], int n, double& V);
void rh(double r[][2], double v[][2], double a[][2], double h, int n);
void vh(double v[][2], double a[][2], double h, int n);
double temp(float a, float b,int n);


int main(void)
{
    int i,j,k;
    double a[N][2], r[N][2], v[N][2], h, T, V;
    ifstream fichIn;
    ofstream fichOPos, fichOE;
    bool div;
    //Generamos aleatoriamente las condiciones iniciales
    //cond_inic_aleatorio(N);
    cond_inic_panal(N);

    //Copiamos las velocidades y posiciones iniciales aleatorias
    fichIn.open("pos-vel_iniciales.txt");
    i=0;
    while(!fichIn.eof() && i<N)
    {
        if(!fichIn.is_open())
            cout << "Error al abrir el fichero";

        fichIn >> r[i][0];
        fichIn >> r[i][1];
        fichIn >> v[i][0];
        fichIn >> v[i][1];
        i++;
    }
    fichIn.close();

    fichOPos.open("particulas_posiciones.txt");
    fichOE.open("particulas_energias.txt");
    //Comprobamos que el número de partículas coincide con la cantidad de datos leídos
    if(i!=N)
        fichOPos << "El número de partículas indicado no coincide con la cantidad de datos iniciales." << endl;
    else
    {
        h=s; k=1;
        fichOPos.precision(6); 
        T=V=0.0;
        //Volcamos las posiciones iniciales
        for(i=0;i<N;i++)
        {
            fichOPos << r[i][0] << ", " << r[i][1] << endl;

            //Calculamos la energía cinética inicial
            if(E)
                T=T+(v[i][0]*v[i][0]+v[i][1]*v[i][1])/2;
        }
        fichOPos << endl;
        fichOE << "0.0000 " << T << " ";

        //Calculamos las aceleraciones (y energía potencial) iniciales
        div=ac(a,r,N,V);
        if(E)
            fichOE << V << " " << T+V <<endl;

        //Aplicamos el algoritmo de Verlet
        while(h<=S && div)
        {
            rh(r,v,a,s,N);
            vh(v,a,s,N);
            div=ac(a,r,N,V);
            vh(v,a,s,N);

            if(k%D==0) //No pasamos todas las posiciones a la simulación
            {
                for(i=0;i<N;i++)
                    fichOPos << r[i][0] << ", " << r[i][1] << endl;
                fichOPos << endl;

                //Escribimos las energías
                if(E)
                {
                    T=0.0;
                    for(i=0;i<N;i++)
                        T=T+(v[i][0]*v[i][0]+v[i][1]*v[i][1])/2;
                    fichOE << h << " " << T << " " << V << " " << T+V << endl;
                }
            }

            h=h+s; k++;
        }

        if (!div) //Comprobamos que no ha habido errores en los cálculos
            fichOPos << "Error al calcular la aceleración" << endl;

    }    

    fichOPos.close();
    fichOE.close();

    //Calculamos la temperatura en el intervalo especificado en el guion
    //cout << temp(20,50,N);

    return 0;
}


//Función cond_inic_aleatorio: genera de forma aleatoria las posiciones y velocidades iniciales de las partículas en la
//caja. Escribe dicha información en el fichero "pos-vel_iniciales.txt" según: en cada línea se escribe posición en
//X, posición en Y, velocidad en X, velocidad en Y
//Argumentos: n, número de partículas
void cond_inic_aleatorio(int n)
{
    int i,j,k;
    double v,p,l,r[N][2],r1[2],r2[2];
    ofstream fichOut;

    v=0.0;
    p=2*Pi/RAND_MAX; //Calculamos la cte por la que hay que multiplicar para generar números aleatorios en [0,2Pi]
    l=1.0*L/RAND_MAX; //Calculamos la cte por la que hay que multiplicar para generar números aleatorios en [0,L]
    fichOut.open("pos-vel_iniciales.txt");
    fichOut.precision(8);
    srand(time(NULL)); //Establecemos la semilla para la generación de números aleatorios

    //Generamos las posiciones y velocidades aleatorias
    for(i=0;i<n;i++)
    {
        r1[0]=rand()*l; r1[1]=rand()*l;
        r[i][0]=r1[0]; r[i][1]=r1[1];
        //Comprobamos que las partículas no parten de posiciones demasiado próximas
        k=0; //Para que no genere infinitos números aleatorios
        for(j=0;j<i;j++)
        {
            r2[0]=r[j][0]; r2[1]=r[j][1];
            while(dist(r1,r2)<R && k<1000)
            {
                r1[0]=rand()*l; r1[1]=rand()*l;
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
            v=rand()*p;
            fichOut << r[i][0] << " " << r[i][1] << " " << cos(v) << " " << sin(v) << endl; 
        }   
        
    }

    fichOut.close();    

    return;
}

//Función cond_inic_panal: inicia las posiciones de las partículas en la caja (rectangular de altura L) y en reposo.
//Escribe dicha información en el fichero "pos-vel_iniciales.txt" según: en cada línea se escribe posición en
//X, posición en Y, velocidad en X, velocidad en Y
//Argumentos: n, número de partículas
void cond_inic_panal(int n)
{
    int H, i;
    double l, a, b;
    ofstream fichOut;

    l=L/3.0;
    a=l*sqrt(3)/2;
    b=l/2;
    H=n/4;
    fichOut.open("pos-vel_iniciales.txt");

    for(i=0;i<H; i++)
    {
        fichOut << a*(2*i+1) << " " << 0.0 << " " << 0.0 << " " << 0.0 << endl;
        fichOut << 2*a*i << " " << b << " " << 0.0 << " " << 0.0 << endl;
        fichOut << 2*a*i << " " << b+l << " " << 0.0 << " " << 0.0 << endl;
        fichOut << a*(2*i+1) << " " << 2*l << " " << 0.0 << " " << 0.0 << endl;
    }


    fichOut.close();   
    return;
}

//Función dist: calcula la distancia entre dos puntos de la caja teniendo en cuenta las condiciones de contorno
//periódicas.
//Argumentos: x, y; vectores de 2-dim que contienen las coordenadas de cada punto.
//Retorno: d, distancia al cubo entre los puntos dados
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
//n, número de partículas del sistema; E, indica si se quiere calcular la energía potencial; V; energía potencial
//del sistema.
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
void rh(double r[][2], double v[][2], double a[][2], double h, int n)
{
    int i,k;
    double d;

    d=h*h/2; //Calculamos las constantes fuera del bucle por optimización
    for (i=0;i<n;i++)
    {
        r[i][0]=r[i][0]+h*v[i][0]+d*a[i][0];
        //Comprobamos que las partículas no chocan con el borde de la caja
        if(r[i][0]<0)
            r[i][0]=r[i][0]+L;
        if(r[i][0]>L)
            r[i][0]=r[i][0]-L;

        r[i][1]=r[i][1]+h*v[i][1]+d*a[i][1];
        //Comprobamos que las partículas no chocan con el borde de la caja
        if(r[i][1]<0)
            r[i][1]=r[i][1]+L;
        if(r[i][1]>L)
            r[i][1]=r[i][1]-L;
    }
    //r[i][0]=r[i][0]-floor(r[i][0]/L)*L;
    return;
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
//energía cinética del sistema del fichero "particulas_energias.txt"
//Argumentos: a,b, extremos del intervalo en el que se calcula la temperatura; n, número de partículas del sistema; 
//Retorno:t, temperatura media del sistema
double temp(float a, float b,int n)
{
    double t,aux,h;
    int i;
    ifstream fichIn;

    t=0.0;
    fichIn.open("particulas_energias.txt");

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
            while(h<=b)
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
