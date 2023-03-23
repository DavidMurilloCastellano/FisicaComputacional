//Supondremos que todos los planetas parten del eje X, por lo que su velocidad inicial solo tendrá dirección Y.

//Incluimos las librerías que necesitamos y definimos constantes universales
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

#define G 6.67e-11 //Cte de gravitación universal
#define M 1.99e30 //Masa del Sol
#define c 1.496e11 //Cte de distancia que aparece en el guion
#define N 9 //Número de cuerpos en nuestro sistema
#define L 40 //Doble del número máximo de cuerpos que se espera en el sistema
#define s 0.05 //Valor del paso que vamos a usar en los desarrollos en serie
#define S 20 //Mayor valor de tiempo que se alcanzará (en las unidades del guion)

//Cabecera con todas las funciones que hemos definido
void reescala(int n);
void PosVelInic(int n);
double dist3(double x[], double y[]);
void ac(double a[], double r[], double m[], int n);
void rh(double r[], double v[], double a[], double h, int n);
void vh(double v[], double a[], double h, int n);
void formato_animacion(void);
int periodo(float T[], double h, int n);

//Función que calcula las posiciones, velocidades y aceleraciones de nuestro modelo en cada instante, escribiéndolas
//en el fichero "pos-vel-acel.txt".
int main(void)
{
    ifstream fichIn, fichIn2;
    ofstream fichOut;
    double r[L], v[L], a[L], m[L/2], h;
    float T[N-1];
    int i,j,k,l;

    //Comprobamos que no se sobrepasa el número de cuerpos
    if (2*N>L)
        cout << "Reduzca el número de cuerpos" << endl;

    else
    {
        
        //Inicializamos a 0 todos los vectores recién definidos
        for (i=0; i<L; i++)
        {
            if (i<N)
                m[i]=0.0;
            r[i]=0.0;
            v[i]=0.0;
            a[i]=0.0;
        }
    
        //Reescalamos los datos de las condiciones iniciales
        //reescala(N);

        //Escribimos las posiciones y velocidades iniciales en el fichero correspondiente
        //PosVelInic(N);

        //Abrimos los ficheros de los que tomamos los datos y en los que vamos a escribir los cálculos posteriores
        //El fichero "pos-vel-acel.txt" se estructura según, cada 3 línea, posiciones, velocidades y aceleraciones
        //en el instante dado, ocupando las columnas impares las coordenadas X y las pares las Y de cada cuerpo 
        fichIn.open("pos-vel_iniciales.txt");
        fichIn2.open("datos.txt");
        fichOut.open("pos-vel-acel.txt");

        //Pasamos al nuevo archivo los datos iniciales
        k=2*N;
        while(!fichIn.eof())
        {
            if(!fichIn.is_open())
                cout << "Error al abrir el fichero";
            for(i=0;i<k;i++)
                fichIn >> r[i]; //Pasamos a nuestro vector las posiciones iniciales
            for(i=0;i<k;i++)
                fichIn >> v[i]; //Pasamos a nuestro vector las velocidades iniciales
        }
        fichIn.close();
    
        fichOut.precision(8); //Establecemos la precisión con la que se escriben los datos en el fichero
        for (i=0; i<k; i++)
            fichOut << fixed << r[i] << " ";
        fichOut << endl;
        for (i=0; i<k; i++)
            fichOut << fixed << v[i] << " ";
        fichOut << endl;
    
        //Leemos las masas de los cuerpos
        i=0;
        while(!fichIn2.eof() && i<N)
        {
            if(!fichIn2.is_open())
                cout << "Error al abrir el fichero";
            fichIn2 >> m[i]; //Pasamos a nuestro vector las masas
            i++;
        }
        fichIn2.close();

        //Calculamos el vector de aceleraciones en el instante inicial y lo escribimos en el fichero
        ac(a,r,m,N);
        for (i=0; i<k; i++)
            fichOut << fixed << a[i] << " ";
        fichOut << endl;
   
        //Calculamos las posiciones, velocidades y aceleraciones de cada cuerpo para los instantes posteriores
        h=s;
        while(h<=S)
        {
            rh(r,v,a,s,N);
            vh(v,a,s,N);
            ac(a,r,m,N);
            vh(v,a,s,N);

            //Escribimos los nuevos vectores en el fichero
            for(i=0;i<k;i++)
                fichOut << fixed << r[i] << " ";
            fichOut << endl;
            for(i=0;i<k;i++)
                fichOut << fixed << v[i] << " ";
            fichOut << endl;
            for(i=0;i<k;i++)
                fichOut << fixed << a[i] << " ";
            fichOut << endl;

            h=h+s;
        }
        fichOut.close();

        //A partir de aquí ya se tiene el fichero "pos-vel-acel.txt" relleno.
        //Escribimos en otro fichero con el formato adecuado para poder usar "animacion_planetas.py"
        formato_animacion();

        //Calculamos los periodos de la órbita de cada cuerpo
        l=periodo(T,s,N); //Actualizar fichero?
        cout << l << endl;
        for(i=0;i<8;i++)
            cout << T[i] << endl;
        
    }
    

    return 0;
}


//Definimos las funciones a las que hemos llamado en el main

//Función reescala: transforma los datos del fichero "condiciones_iniciales.txt", dispuestos en filas según: masa,
//distancia media al Sol, velocidad orbital (ordenados según proximidad al Sol y expresados en unidades SI), en los
//correspondientes reescalados según se indica en el guion. Solo se necesita emplear la primera vez que se ejecuta 
//el programa. Los datos se han obtenido de: https://nssdc.gsfc.nasa.gov/planetary/factsheet/
//Argumentos: n, número de cuerpos que consideramos en nuestro sistema
void reescala(int n)
{
    ifstream fichIn;
    ofstream fichOut;
    double datos[3][n], cte;
    int i,j;

    //Leemos los datos
    fichIn.open("condiciones_iniciales.txt");
    fichOut.open("datos.txt");
    n--;
    while(!fichIn.eof())
    {
        if(!fichIn.is_open())
            cout << "Error al abrir el fichero";
        for (i=0; i<=2; i++)
        {
            for(j=0; j<=n; j++)
                fichIn >> datos[i][j];
        }
    }
    
    //Aplicamos la transformación correspondiente a cada dato
   cte=sqrt(G*M/(c*c*c));
   for(j=0;j<=n;j++)
   {
        datos[0][j]=datos[0][j]/M;
        datos[1][j]=datos[1][j]/c;
        datos[2][j]=datos[2][j]/(c*cte);
   }

    //Pasamos los datos reescalados a un nuevo fichero
    fichOut.precision(15); //Establecemos la precisión con la que se escriben los datos en el fichero
    for (i=0; i<=2; i++)
        {
            for(j=0; j<=n; j++)
                fichOut << fixed << datos[i][j] << " ";
            fichOut << endl;
        }

    fichOut.close();
    fichIn.close();
    return;
}

//Función PosInic: lee del fichero "datos.txt" las distancias medias de cada planeta al Sol y vuelca en otro
//fichero "pos-vel_iniciales.txt" la posición y velocidad inicial de cada planeta, suponiendo que todos
//están alineados en t=0 en el eje X. En "pos-vel_iniciales.txt" aparecen en la primera fila las coordenadas de
//la posición inicial de cada planeta de la forma "x y" ordenados por proximidad al Sol. Análogo para la velocidad
//en la segunda línea (todo en las unidades reescaladas)
//Argumentos: n, número de cuerpos que consideramos en nuestro sistema
void PosVelInic(int n)
{
    ifstream fichIn;
    ofstream fichOut;
    double datos[2][2*n];
    int i,j,m;

    //Inicializamos la matriz sabiendo que todos los planetas empiezan con y=0, v_x=0
    m=2*n-1;
    n--;
    for(j=0;j<=m;j=j+2)
    {
        datos[0][j+1]=0.0;
        datos[1][j]=0.0;
    }

    fichIn.open("datos.txt");
    fichOut.open("pos-vel_iniciales.txt");
    while(!fichIn.eof())
    {
        if(!fichIn.is_open())
            cout << "Error al abrir el fichero";

        for(j=0;j<=n;j++)
            fichIn >> datos[0][0]; //Descartamos las lecturas de las masas

        for(j=0;j<=m;j=j+2)
            fichIn >> datos[0][j]; //Leemos las posiciones (eje X)

        for(j=1;j<=m;j=j+2)
            fichIn >> datos[1][j]; //Leemos las velocidades (sentido positivo del eje Y, giro antihorario)
    }

    //Volcamos al nuevo fichero
    fichOut.precision(8);
    for(i=0;i<=1;i++)
    {
        for(j=0;j<=m;j++)
            fichOut << fixed << datos[i][j] << " ";
        fichOut << endl;
    }
    
    fichIn.close();
    fichOut.close();
    return;
}

//Función dist3: calcula la distancia al cubo entre dos puntos del espacio.
//Argumentos: x, y; vectores de 2-dim que contienen las coordenadas de cada punto.
//Retorno: d, distancia al cubo entre los puntos dados
double dist3(double x[], double y[])
{
    double d;
    d=sqrt((x[0]-y[0])*(x[0]-y[0])+(x[1]-y[1])*(x[1]-y[1]));
    d=d*d*d;
    return d;
}

//Función ac: calcula la aceleración a la que está sometida cada cuerpo en un instante determinado a partir
//de la ley de gravitación de Newton en las unidades en las que se está trabajando (G=1).
//Argumentos: a,r,m; vectores de aceleración, posición y masa de cada cuerpo en el instante considerado, respect;
//n, número de cuerpos del sistema
void ac(double a[], double r[], double m[], int n)
{
    double acel[2], r1[2], r2[2], cte, d; //r1 es el vector r_i; r2 es el r_j
    int i,j,k;

    k=2*n;
    for (i=0;i<k;i=i+2)
    {
        acel[0]=0.0; acel[1]=0.0; //Inicializamos la suma
        r1[0]=r[i]; r1[1]=r[i+1];
        for(j=0;j<k;j=j+2)
        {
            if(j!=i)
            {
                r2[0]=r[j]; r2[1]=r[j+1];
                d=dist3(r1,r2);
                if(d==0) //Comprobamos que no dividimos entre 0
                {
                    acel[0]=1;
                    acel[1]=1;
                    j=k+1;
                }
                else
                {
                    cte=m[j/2]/d;
                    acel[0]=acel[0]+cte*(r2[0]-r1[0]); //Las componentes pares corresponden al eje X
                    acel[1]=acel[1]+cte*(r2[1]-r1[1]); //Las componentes impares corresponden al eje Y
                }
                
            }
        }
        a[i]=acel[0]; a[i+1]=acel[1]; //Pasamos al vector aceleración el valor final
    }

    return;
}

//Función rh: calcula las posiciones en un instante h posterior a partir del desarrollo en serie
//Argumentos: r, v, a, vectores posiciones, velocidades y aceleraciones en el instante anterior;
//h, paso; n, número de cuerpos
void rh(double r[], double v[], double a[], double h, int n)
{
    int i,k;
    double d;

    k=2*n;
    d=h*h/2; //Calculamos las constantes fuera del bucle por optimización
    for (i=0;i<k;i=i+2)
    {
        r[i]=r[i]+h*v[i]+d*a[i];
        r[i+1]=r[i+1]+h*v[i+1]+d*a[i+1];
    }

    return;
}

//Función vh: calcula los dos primeros sumandos de la velocidad en un instante h posterior a partir del desarrollo 
//en serie
//Argumentos: v, a, vectores posiciones, velocidades y aceleraciones en el instante anterior;
//h, paso; n, número de cuerpos
void vh(double v[], double a[], double h, int n)
{
    int i,k;
    float d;

    k=2*n;
    d=h/2;
    for (i=0;i<k;i=i+2)
    {
        v[i]=v[i]+d*a[i];
        v[i+1]=v[i+1]+d*a[i+1];
    }

    return;
}

//Función formato_animacion: vuelca las posiciones generadas por el programa "planetas.cpp" en el fichero 
//"planets_data.dat" con el formato apropiado para luego usar el código de "animacion_planetas.py"
void formato_animacion(void)
{
    ifstream fichIn;
    ofstream fichOut;
    double r[L];
    int i, j, k;

    fichIn.open("pos-vel-acel.txt");
    fichOut.open("planets_data.dat");

    k=2*N; j=1;
    fichOut.precision(8);
    while(!fichIn.eof())
    {
        if(!fichIn.is_open())
            cout << "Error al abrir el fichero";
        if(j%3==1) //Comprobamos que vamos a copiar solo las posiciones
        {
            for(i=0;i<k;i++)
                fichIn >> r[i]; //Copiamos los datos del fichero de entrada a nuestro vector
            for(i=0;i<k;i=i+2)
            {
                fichOut << fixed << r[i] << ", " << r[i+1] << endl; //Lo pasamos al fichero de salida formateado
            }
            fichOut << endl;
        }
        else //Descartamos el resto de medidas
        {
            for(i=0;i<k;i++)
                fichIn >> r[0];
        }
        j++;
    }

    fichIn.close();
    fichOut.close();

    return;
}

//Función periodo: calcula el periodo de rotación (en días) de cada cuerpo a partir de sus posiciones. Como todos
//parten de y=0, completarán una vuelta cuando su coordenada y pase de ser negativa a mayor o igual que 0. También 
//se tiene en cuenta que los planetas más próximos al Sol tienen menor periodo de traslación. No se calcula el
//periodo del Sol.
//Argumentos: T[], vector de periodos; h, paso con el que se ha aplicado el algoritmo; n, número de cuerpos
//Retorno: i, número de planetas que completan un periodo con los datos del fichero "planets_data.dat"
int periodo(float T[], double h, int n) 
{
    float aux[n-1][2], t;
    int i,j;
    ifstream fichIn;

    //Inicializamos a 0 la primera columna de la matriz y el vector de periodos
    n--; t=0.0;
    for(j=0;j<n;j++)
        aux[j][0]=0.0;

    fichIn.open("planetas_posiciones.txt"); //Fichero con los datos de "planets_data.dat" pero sin comas
    i=0;

    while(i<n && !fichIn.eof())
    {
        if(!fichIn.is_open())
            cout << "Error al abrir el fichero";
        //Pasamos los datos de la iteración anterior a la segunda columna para aquellos planetas que no han dado
        //una vuelta todavía
        for(j=i;j<n;j++)
            aux[j][1]=aux[j][0];
        //Leemos los datos de la siguiente iteación
        //Ignoramos las lecturas de la posición del Sol y los cuerpos que ya han dado una vuelta
        for(j=0;j<=i;j++)
        {
            fichIn >> aux[i][0]; fichIn >> aux[i][0];
        }
        //Guardamos las medidas de la siguiente iteración de los planetas que no han dado una vuelta
        for(j=i;j<n;j++)
        {
            fichIn >> aux[j][0]; fichIn >> aux[j][0]; //Nos quedamos con la lectura en Y
            if(aux[j][1]<0 && aux[j][0]>=0)
            {
                i++;
                T[j]=t;
            }
        }
        t=t+h;
    }

    //Pasamos a días (terrestres) los datos obtenidos e indicamos con 0 los planetas que no completaron una vuelta
    for(j=0;j<i;j++)
        T[j]=58.129*T[j];
    for(j=i;j<n;j++)
        T[j]=0;

    fichIn.close();
    return i;
}
//Hay que comprobar: órbitas son elípticas, los periodos de rotación son similares a los reales, la energía se
//conserva y las órbitas son estables frente a perturbaciones de las condiciones iniciales.
