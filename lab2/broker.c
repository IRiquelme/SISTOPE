#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "particula.h"
#include <time.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

void cerrar_pipes(int workers, int fdWorkerToBroken[][2], int fdBrokenToWorker[][2])
{
    int i;
    for (i = 0; i < workers; i++)
    {
        close(fdWorkerToBroken[i][0]);
        close(fdBrokenToWorker[i][1]);
    }
}

void crear_pipes(int workers, int fdWorkerToBroken[][2], int fdBrokenToWorker[][2])
{
    int i;
    for (i = 0; i < workers; i++)
    {
        if (pipe(fdWorkerToBroken[i]) == -1 || pipe(fdBrokenToWorker[i]) == -1)
        {
            printf("Error al crear un pipe\n");
            exit(0);
        }
    }
}

void crear_workers(int workers, int fdWorkerToBroken[][2], int fdBrokenToWorker[][2], char *argv[])
{
    int i;
    for (i = 0; i < workers; i++)
    {
        int pid = fork();

        if (pid == 0)
        {
            close(fdWorkerToBroken[i][0]);
            close(fdBrokenToWorker[i][1]);

            dup2(fdWorkerToBroken[i][1], STDOUT_FILENO);
            dup2(fdBrokenToWorker[i][0], STDIN_FILENO);

            char *args[] = {"./worker", argv[1], NULL};
            execv(args[0], args);
        }
        else
        {
            close(fdWorkerToBroken[i][1]);
            close(fdBrokenToWorker[i][0]);
        }
    }
}

void procesar_respuesta_workers(int celdas, int workers, double *arregloCeldas, int fdWorkerToBroken[][2], int *lineasTrabajadasPorWorker)
{
    int i;
    char archivoWorker[50];

    for (i = 0; i < workers; i++)
    {
        int lineasTrabajadas;
        read(fdWorkerToBroken[i][PIPE_READ], &lineasTrabajadas, sizeof(int));
        lineasTrabajadasPorWorker[i] = lineasTrabajadas;

        read(fdWorkerToBroken[i][PIPE_READ], archivoWorker, sizeof(archivoWorker));
        FILE *fp = fopen(archivoWorker, "r");
        int j;
        for (j = 0; j < celdas; j++)
        {
            double valor;
            fscanf(fp, "%lf", &valor);
            arregloCeldas[j] += valor;
        }
        fclose(fp);
        remove(archivoWorker);
    }
}

void mostrar_resultado(int *lineasTrabajasPorWorker, double *arregloCeldas, int celdas, int workers)
{
    int i, j;
    for (i = 0; i < workers; i++)
    {
        printf("Worker %d: %d lineas procesadas\n", i + 1, lineasTrabajasPorWorker[i]);
    }

    for (i = 0; i < celdas; i++)
    {
        printf("%d %lf |", i, arregloCeldas[i]);
        for (j = 0; j < arregloCeldas[i] / 50; j++)
        {
            printf("o");
        }
        printf("\n");
    }
}

void asignar_trabajo(const char *input, int chunks, int workers, int fdBrokenToWorker[][2])
{
    int cantidadParticulas, workerRandom, i;
    int lineasLeidas = 0;
    char buffer[50];

    srand(time(NULL)); // Para variar la semilla del random en cada ejecucion

    FILE *archivoParticulas = fopen(input, "r");
    if (archivoParticulas == NULL)
    {
        printf("Error al abrir el archivo de entrada\n");
        exit(0);
    }

    fscanf(archivoParticulas, "%d", &cantidadParticulas);

    fgets(buffer, sizeof(buffer), archivoParticulas); // REVISAR ESTO

    while (lineasLeidas < cantidadParticulas)
    {
        if ((lineasLeidas % chunks) == 0)
        {
            workerRandom = rand() % workers;
        }
        lineasLeidas++;
        fgets(buffer, sizeof(buffer), archivoParticulas);
        write(fdBrokenToWorker[workerRandom][PIPE_WRITE], buffer, sizeof(buffer));
    }

    fclose(archivoParticulas);

    for (i = 0; i < workers; i++)
    {
        write(fdBrokenToWorker[i][PIPE_WRITE], "FIN", sizeof("FIN"));
    }
}

int obtener_maximo(double *arregloCeldas, int celdas)
{
    int i;
    int celdaEnergiaMaxima = 0;
    for (i = 0; i < celdas; i++)
    {
        if (arregloCeldas[i] > arregloCeldas[celdaEnergiaMaxima])
        {
            celdaEnergiaMaxima = i;
        }
    }
    return celdaEnergiaMaxima;
}

void crear_archivo_salida(double *arregloCeldas, int celdas, const char *output)
{
    FILE *archivoSalida = fopen(output, "w");
    if (archivoSalida == NULL)
    {
        printf("Error al crear el archivo de salida\n");
        exit(0);
    }

    int i;
    int celdaEnergiaMaxima = obtener_maximo(arregloCeldas, celdas);
    fprintf(archivoSalida, "%d %lf\n", celdaEnergiaMaxima, arregloCeldas[celdaEnergiaMaxima]);
    for (i = 0; i < celdas; i++)
    {
        fprintf(archivoSalida, "%d %lf\n", i, arregloCeldas[i]);
    }

    fclose(archivoSalida);
}

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