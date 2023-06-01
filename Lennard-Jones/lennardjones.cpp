//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
//#include <iomanip>
using namespace std;

#define Pi 3.14159265358979 //Valor del número Pi
#define N 20 //Número de átomos que conforman nuestro sistema
#define L 10 //Tamaño (en las unidades consideradas) de cada lado de la caja cuadrada
#define s 2e-4 //Paso con el que se aplica el algoritmo
#define S 50 //Límite de tiempo hasta el que se considera la simulación
#define D 100 //Número de líneas que se pasan al fichero para crear el vídeo de la simulación
#define R 1 //Separación mínimia inicial entre cada par de partículas
#define E true //Indica si se desea calcular o no la energía del sistema

//Cabecera con todas las funciones que hemos definido
void cond_inic_aleatorio(int n);
void cond_inic_panal(int n);
double dist(double x[], double y[]);
bool ac(double a[][2], double r[][2], int n, double& V);
void rh(double r[][2], double v[][2], double a[][2], double h, int n);
void vh(double v[][2], double a[][2], double h, int n);
double temp(float a, float b,int n);
void HistVel(float a, float b, int n, double T);


int main(void)
{
    int i,j,k;
    double a[N][2], r[N][2], v[N][2], v2, h, T, V, t;
    ifstream fichIn;
    ofstream fichOPos, fichOVel, fichOE;
    bool div;

    //Generamos aleatoriamente las condiciones iniciales
    //cond_inic_aleatorio(N);
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

    fichOPos.open("particulas_posiciones.txt");
    fichOVel.open("particulas_velocidades.txt");
    fichOE.open("particulas_energias.txt");
    //Comprobamos que el número de partículas coincide con la cantidad de datos leídos
    if(i!=N)
        fichOPos << "El número de partículas indicado no coincide con la cantidad de datos iniciales." << endl;
    else
    {
        h=s; k=1;
        fichOPos.precision(6); 
        fichOVel.precision(6);
        T=V=0.0;
        //Volcamos las posiciones y velocidades iniciales
        fichOVel << "0.00" << endl;
        for(i=0;i<N;i++)
        {
            v2=v[i][0]*v[i][0]+v[i][1]*v[i][1];
            fichOPos << r[i][0] << ", " << r[i][1] << endl;
            fichOVel << v[i][0] << " " << v[i][1] << " " << sqrt(v2) << endl;

            //Calculamos la energía cinética inicial
            if(E)
                T=T+v2/2;
        }
        fichOPos << endl;
        fichOE << "0.00 " << T << " ";

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
                fichOVel << h << endl;
                T=0.0;
                for(i=0;i<N;i++)
                {
                    v2=v[i][0]*v[i][0]+v[i][1]*v[i][1];
                    fichOPos << r[i][0] << ", " << r[i][1] << endl;
                    fichOVel << v[i][0] << " " << v[i][1] << " " << sqrt(v2) << endl;
                    if(E)
                        T=T+v2/2;
                }
                fichOPos << endl;
                if(E)
                    fichOE << h << " " << T << " " << V << " " << T+V << endl;
            }

            h=h+s; k++;
        }

        if (!div) //Comprobamos que no ha habido errores en los cálculos
            fichOPos << "Error al calcular la aceleración" << endl;

    }    

    fichOPos.close();
    fichOVel.close();
    fichOE.close();

    //Calculamos la temperatura en el intervalo especificado en el guion
    t=temp(20,50,N);

    //Histograma de velocidades
    HistVel(20,50,N,t);

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

//REVISAR
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
//energía cinética del sistema del fichero "particulas_energias.txt" y escribe la temperatura obtenida en
//particulas_temperatura.txt.
//Argumentos: a,b, extremos del intervalo en el que se calcula la temperatura; n, número de partículas del sistema; 
double temp(float a, float b,int n)
{
    double t,aux,h;
    int i;
    ifstream fichIn;
    ofstream fichOut;

    t=0.0;
    fichIn.open("particulas_energias.txt");
    fichOut.open("particulas_temperatura.txt");

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

    fichOut << t;
    
    fichIn.close();
    fichOut.close();

    return t;
}

//Función HistVel: pasa a un fichero la distribución relativa de las partículas según el promedio, en el instante
//de tiempo especificado, de la velocidad que llevan en cada componente y el módulo.
//Argumentos: a,b, límites del intervalo donde se calcula el promedio; n, número de partículas; T, temperatura del
//sistema
void HistVel(float a, float b, int n, double T)
{
    float t, limS, limI, vMm, aux, lx, ly, lm, vx[N], vy[N], vm[N], tem;
    int i, j, M, v[4];
    ifstream fichIn;
    ofstream fichOut;

    //Iniciamos los datos
    t=0.0;
    tem=sqrt(T);
    //vMx=vMy=vMm=0.0;
    for(i=0;i<n;i++)
    {
        vx[i]=vy[i]=vm[i]=0.0;
    }
    fichIn.open("particulas_velocidades.txt");
    fichOut.open("histograma-velocidades.txt");

    if(a>=0 && b<=S && a<b) //Se comprueba que los límites están dentro del rango de tiempo calculado
    {
        if(!fichIn.is_open())
            t=0.0;
        else
        {
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
                for(i=0;i<n; i++)
                {
                    fichIn >> aux;
                    vx[i]=vx[i]+aux;

                    fichIn >> aux;
                    vy[i]=vy[i]+aux;

                    fichIn >> aux;
                    vm[i]=vm[i]+aux;
                }

                if(!fichIn.eof())
                    fichIn >> t;             
            }

            //Calculamos el promedio de la velocidad de cada partícula y el vector con el promedio del módulo de
            //la velocidad de cada partícula
            M=floor((b-a)/(s*D))+1;
            for(i=0;i<n;i++)
            {
                vx[i]=vx[i]/M;
                vy[i]=vy[i]/M;
                vm[i]=vm[i]/M;
            }

            //Dividimos la recta real para obtener las particiones óptimas para poder aplicar el test de chi^2
            //En X e Y, la distribución es una gaussiana, luego dividimos la recta real en cuatro subintervalos
            //equiprobables (0.25)
            limS=0.6745*tem;
            limI=-limS;
            //Iniciamos el vector con el que vamos a contar
            for(i=0;i<3;i++)
                v[i]=0;
            //Eje X
            for(i=0;i<n;i++)
            {
                if(vx[i]<limI)
                    v[0]++;
                else if(vx[i]<0)
                    v[1]++;
                else if(vx[i]<limS)
                    v[2]++;
                else
                    v[3]++;
            }
            //Pasamos la información obtenida al fichero para poder representarla luego
            //Tomamos como puntos medios de los intervalos los que dejan a ambos lados probabilidad 0.125
            fichOut << -1.1505*tem << ", " << (1.0*v[0])/n << endl;
            fichOut << -0.3187*tem << ", " << (1.0*v[1])/n << endl;
            fichOut << 0.3187*tem << ", " << (1.0*v[2])/n << endl;
            fichOut << 1.1505*tem << ", " << (1.0*v[3])/n << endl;
            fichOut << endl;
            //Iniciamos el vector con el que vamos a contar
            for(i=0;i<3;i++)
                v[i]=0;
            //Eje Y
            for(i=0;i<n;i++)
            {
                if(vy[i]<limI)
                    v[0]++;
                else if(vy[i]<0)
                    v[1]++;
                else if(vy[i]<limS)
                    v[2]++;
                else
                    v[3]++;
            }
            //Pasamos la información obtenida al fichero para poder representarla luego
            //Tomamos como puntos medios de los intervalos los que dejan a ambos lados probabilidad 0.125
            fichOut << -1.1505*tem << ", " << (1.0*v[0])/n << endl;
            fichOut << -0.3187*tem << ", " << (1.0*v[1])/n << endl;
            fichOut << 0.3187*tem << ", " << (1.0*v[2])/n << endl;
            fichOut << 1.1505*tem << ", " << (1.0*v[3])/n << endl;
            fichOut << endl;

            //Pasamos ahora a la distribución de los módulos. En este caso, siguen una distribución de Maxwell
            //De nuevo, partimos la semirrecta real positiva en cuatro subintervalos equiprobables
            limI=0.7786*sqrt(2)*tem; //Deja a la izquierda 0.25
            aux=1.0877*sqrt(2)*tem; //Deja a la izquierda 0.5
            limS=1.4332*sqrt(2)*tem; //Deja a la izquierda 0.75
            //Iniciamos el vector con el que vamos a contar
            for(i=0;i<3;i++)
                v[i]=0;
            //Módulo
            for(i=0;i<n;i++)
            {
                if(vm[i]<limI)
                    v[0]++;
                else if(vm[i]<aux)
                    v[1]++;
                else if(vm[i]<limS)
                    v[2]++;
                else
                    v[3]++;
            }
            //Pasamos la información obtenida al fichero para poder representarla luego
            //Tomamos como puntos medios de los intervalos los que dejan a ambos lados probabilidad 0.125
            fichOut << 0.5884*sqrt(2)*tem << ", " << (1.0*v[0])/n << endl;
            fichOut << 0.9365*sqrt(2)*tem << ", " << (1.0*v[1])/n << endl;
            fichOut << 1.247*sqrt(2)*tem << ", " << (1.0*v[2])/n << endl;
            fichOut << 1.694*sqrt(2)*tem << ", " << (1.0*v[3])/n << endl;
            fichOut << endl;



            /*
            //Calculamos el máximo de cada vector para poder hacer equiparticiones de cada intervalo
            vMx=abs(vx[0]);
            vMy=abs(vy[0]);
            vMm=vm[0];
            for(i=1;i<n;i++)
            {
                if(abs(vx[i])>vMx)
                    vMx=abs(vx[i]);
                if(abs(vy[i])>vMy)
                    vMy=abs(vy[i]);
                if(vm[i]>vMm)
                    vMm=vm[i];
            }

            lx=2*vMx/n; ly=2*vMy/n; //Hay valores positivos y negativos
            lm=vMm/n;

            //Contamos cuántas partículas hay en cada subintervalo de velocidades

            //Iniciamos el vector con el que vamos a contar
            for(i=0;i<n;i++)
                v[i]=0;
            //Eje X
            for(i=0;i<n;i++)
            {
                aux=vx[i]+vMx;
                j=floor(aux/lx);
                if(j>=n)
                    j=n-1;
                if(j<0)
                    j=0;
                v[j]++;
            }
            //Pasamos al fichero la información obtenida
            for(i=0;i<n;i++)
                fichOut << lx*(i+1/2)-vMx << ", " << (v[i]*1.0)/n << endl;
            fichOut << endl;


            //Iniciamos el vector con el que vamos a contar
            for(i=0;i<n;i++)
                v[i]=0;
            //Eje Y
            for(i=0;i<n;i++)
            {
                aux=vy[i]+vMy;
                j=floor(aux/ly);
                if(j>=n)
                    j=n-1;
                if(j<0)
                    j=0;
                v[j]++;
            }
            //Pasamos al fichero la información obtenida
            for(i=0;i<n;i++)
                fichOut << ly*(i+1/2)-vMy << ", " << (v[i]*1.0)/n << endl;
            fichOut << endl;


            //Iniciamos el vector con el que vamos a contar
            for(i=0;i<n;i++)
                v[i]=0;
            //Módulo
            for(i=0;i<n;i++)
            {
                j=floor(vm[i]/lm);
                if(j>=n)
                    j=n-1;
                v[j]++;
            }
            //Pasamos al fichero la información obtenida
            for(i=0;i<n;i++)
                fichOut << lm*(i+1/2) << ", " << (v[i]*1.0)/n << endl;
            fichOut << endl;
            */
        }
    }
    fichOut.close();

    return;
}
