#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "particula.h"

double formula(int celdas, double energiaInicial, int posicionCelda, double energiaParticula, int posicionParticula)
{
    double energiaRepartida;
    double energiaMinima;

    energiaRepartida = (pow(10.0, 3.0) * energiaParticula) / (celdas * (double)(sqrt(abs(posicionParticula - posicionCelda) + 1.0)));
    energiaMinima = pow(10.0, -3.0) / celdas;

    // Si la energia repartida es mayor a la energia minima, entonces si se le suma a la celda y si no, entonces se retorna la energia inicial
    if (energiaRepartida >= energiaMinima)
    {
        return energiaInicial + energiaRepartida;
    }
    else
    {
        return energiaInicial;
    }
}

int main(int argc, char const *argv[])
{
    int celdas = atoi(argv[1]);  //  numero de celdas

    int tamañoArreglo = celdas + 1;
    double *arregloCeldas = (double *)malloc(tamañoArreglo * sizeof(double)); 

    char buffer[25];
    int lineasTrabajadas = 0;
    int j;
    while (1)
    {
        if (read(STDIN_FILENO, buffer, sizeof(buffer)) > 0)
        {
            if (strcmp(buffer, "FIN") == 0)
            {
                break;
            }

            int energiaParticula;
            int posicionParticula;

            sscanf(buffer, "%d %lf", &energiaParticula, &posicionParticula);

            for (j = 1; j <= celdas; j++){
            arregloCeldas[j] = formula(celdas, arregloCeldas[j], j, energiaParticula, posicionParticula);
            }

            lineasTrabajadas++;
        }
    }

    arregloCeldas[0] = (double) lineasTrabajadas;
    write(STDOUT_FILENO, arregloCeldas, sizeof(double) * tamañoArreglo);

    return 0;
}