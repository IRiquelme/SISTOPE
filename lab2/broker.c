#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "particula.h"
#include "fbroker.c"
#include <time.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

int main(int argc, char *argv[])
{
    int celdas = atoi(argv[1]);  //  numero de celdas
    int workers = atoi(argv[2]); //  numero de workers
    int D = atoi(argv[3]);       //  D indica si se imprime por consola el resultado
    int chunks = atoi(argv[4]);  //  numero de chunks

    char input[100]; //  i es el nombre del archivo de entrada
    strcpy(input, argv[5]);

    char output[100]; //  o es el nombre del archivo de salida
    strcpy(output, argv[6]);

    int fdWorkerToBroken[workers][2]; // Matriz para los pipes con mensajes para el broker (Broker lee; Workers escriben)
    int fdBrokenToWorker[workers][2]; // Matriz para los pipes con mensajes para el worker (Worker lee; Broker escribe)

    crear_pipes(workers, fdWorkerToBroken, fdBrokenToWorker);

    crear_workers(workers, fdWorkerToBroken, fdBrokenToWorker, argv);

    asignar_trabajo(input, chunks, workers, fdBrokenToWorker);

    wait(NULL);

    double *arregloCeldas = (double *)malloc(celdas * sizeof(double));
    int *lineasTrabajasPorWorker = (int *)malloc(workers * sizeof(int));

    procesar_respuesta_workers(celdas, workers, arregloCeldas, fdWorkerToBroken, lineasTrabajasPorWorker);

    cerrar_pipes(workers, fdWorkerToBroken, fdBrokenToWorker);

    crear_archivo_salida(arregloCeldas, celdas, output);

    if (D)
    {
        mostrar_resultado(lineasTrabajasPorWorker, arregloCeldas, celdas, workers);
    }

    free(arregloCeldas);
    free(lineasTrabajasPorWorker);
    return 0;
}