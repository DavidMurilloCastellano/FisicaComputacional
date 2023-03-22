#include <iostream>
#include <fstream>
using namespace std;

#define N 9 //Número de cuerpos en nuestro sistema
#define L 40 //Doble del número máximo de cuerpos que se espera en el sistema

//Función que vuelca las posiciones generadas por el programa "planetas.cpp" en el fichero "planets_data.dat" con
//el formato apropiado para luego usar el código de "animacion_planetas.py"
int main(void)
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

    return 0;
}