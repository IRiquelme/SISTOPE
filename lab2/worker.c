#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "fworker.h"

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

    generar_archivo_resultado(arregloCeldas, celdas, fileName);
    write(STDOUT_FILENO, &lineasTrabajadas, sizeof(int));
    write(STDOUT_FILENO, fileName, sizeof(fileName));
    free(arregloCeldas);
    return 0;
}