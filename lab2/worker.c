#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
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

void arrayToFile(double *arreglo, int celdas, char *fileName)
{
    FILE *fp = fopen(fileName, "a");
    int i;
    for (i = 0; i < celdas; i++)
    {
        fprintf(fp, "%lf\n", arreglo[i]);
    }
    fclose(fp);
}

int main(int argc, char const *argv[])
{
    int celdas = atoi(argv[1]);
    int pid = getpid();
    char fileName[50];
    sprintf(fileName, "worker%d.txt", pid);

    double *arregloCeldas = (double *)malloc(celdas * sizeof(double)); 

    char buffer[50];
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

            int posicionParticula;
            double energiaParticula;

            sscanf(buffer, "%d %lf", &posicionParticula, &energiaParticula);

            for (j = 0; j < celdas; j++){
            arregloCeldas[j] = formula(celdas, arregloCeldas[j], j, energiaParticula, posicionParticula);
            }

            lineasTrabajadas++;
        }
    }
    FILE *fp = fopen(fileName, "w");
    for (j = 0; j < celdas; j++)
    {
        fprintf(fp, "%lf\n", arregloCeldas[j]);
    }

    arrayToFile(arregloCeldas, celdas, fileName);
    write(STDOUT_FILENO, &lineasTrabajadas, sizeof(int));
    write(STDOUT_FILENO, fileName, sizeof(fileName));
    free(arregloCeldas);
    return 0;
}