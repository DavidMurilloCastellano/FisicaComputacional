//Incluimos las librerías que necesitamos y definimos constantes universales (en S.I.)
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

#define G 6.67e-11 //Cte de gravitación universal
#define MT 5.9736e24 //Masa de la Tierra
#define ML 0.07349e24 //Masa de la Luna
#define d 3.844e8 //Distancia Tierra-Luna
#define w 2.6617e-6 //Periodo de rotación de la Luna en torno a la Tierra
#define RT 6.378160e6 //Radio de la Tierra
#define LT 1.7374e6 //Radio de la Luna
#define ve 11.2e3 //Velocidad de escape de la Tierra
#define h 60 //Paso temporal con el que se aplicará el algoritmo
#define T 1e8 //Instante hasta el que se aplica el algoritmo
#define A 10 //Número de pasos que omiten antes de pasar al fichero


//Cabecera con todas las funciones que hemos definido
//y1=r, y2=p_r, y3=phi, y4=p_phi
double f1(double y2); //ED de r
double f2(double t, double y1, double y3, double y4, double D, double mu); //ED de p_r
double f3(double y1, double y4); //ED de phi
double f4(double t, double y1, double y3, double D, double mu); //ED de p_phi


int main (void)
{
    double D, mu, y0[4], y[4], k[4][4], t;
    int i,j;
    ofstream fichO;

    //Calculamos las constantes
    D=G*MT/(d*d*d);
    mu=ML/MT;

    //Indicamos las condiciones iniciales
    y[0]=RT/d;
    y[1]=ve/d;
    y[2]=0.5;
    y[3]=0;

    //Escribimos las condiciones iniciales en el fichero de salida
    fichO.open("trayectorias.txt");
    fichO << y[0]*cos(y[2]) << ", " << y[0]*sin(y[2]) << endl;
    fichO << 1 << ", " << 0 << endl << endl; 

    t=0;
    j=0;

    //Aplicamos el algoritmo y pasamos al fichero para la representación posterior
    while(t<=T)
    {
        j++;
        //Copiamos el nuevo vector de condiciones iniciales
        for(i=0;i<4;i++)
            y0[i]=y[i];
    
        //Calculamos los coeficientes
        k[0][0]=h*f1(y0[1]);
        k[0][1]=h*f2(t,y0[0],y0[2],y0[3],D,mu);
        k[0][2]=h*f3(y0[0],y0[3]);
        k[0][3]=h*f4(t,y0[0],y0[2],D,mu);

        k[1][0]=h*f1(y0[1]+k[0][1]/2);
        k[1][1]=h*f2(t+h/2,y0[0]+k[0][0]/2,y0[2]+k[0][2]/2,y0[3]+k[0][3]/2,D,mu);
        k[1][2]=h*f3(y0[0]+k[0][0]/2,y0[3]+k[0][3]/2);
        k[1][3]=h*f4(t+h/2,y0[0]+k[0][0]/2,y0[2]+k[0][2]/2,D,mu);

        k[2][0]=h*f1(y0[1]+k[1][1]/2);
        k[2][1]=h*f2(t+h/2,y0[0]+k[1][0]/2,y0[2]+k[1][2]/2,y0[3]+k[1][3]/2,D,mu);
        k[2][2]=h*f3(y0[0]+k[1][0]/2,y0[3]+k[1][3]/2);
        k[2][3]=h*f4(t+h/2,y0[0]+k[1][0]/2,y0[2]+k[1][2]/2,D,mu);

        k[3][0]=h*f1(y0[1]+k[2][1]);
        k[3][1]=h*f2(t+h,y0[0]+k[2][0],y0[2]+k[2][2],y0[3]+k[2][3],D,mu);
        k[3][2]=h*f3(y0[0]+k[2][0],y0[3]+k[2][3]);
        k[3][3]=h*f4(t+h,y0[0]+k[2][0],y0[2]+k[2][2],D,mu);

        //Calculamos el valor de cada variable en el instante t+h
        for(i=0;i<4;i++)
            y[i]=y0[i]+(k[0][i]+k[1][i]+k[2][i]+k[3][i])/6;
        
        //Pasamos los datos al fichero cuando se indique
        t=t+h;

        if(j%A==0)
        {
            fichO << y[0]*cos(y[2]) << ", " << y[0]*sin(y[2]) << endl;
            fichO << cos(w*t) << ", " << sin(w*t) << endl << endl;
        }
        
    }

    fichO.close();

    return 0;
}

double f1(double y2)
{
    return y2;
}

double f2(double t, double y1, double y3, double y4, double D, double mu)
{
    double r, aux, Cos;

    aux=y1*y1;
    Cos=cos(y3-w*t);
    r=sqrt(1+aux-2*y1*Cos);
    r=r*r*r;
    return y4*y4/(aux*y1)-D*(1/aux+mu*(y1-Cos)/r);
}

double f3(double y1, double y4)
{
    return y4/(y1*y1);
}

double f4(double t, double y1, double y3, double D, double mu)
{
    double r;

    r=sqrt(1+y1*y1-2*y1*cos(y3-w*t));
    r=r*r*r;
    return -D*mu*y1/r*sin(y3-w*t);
}