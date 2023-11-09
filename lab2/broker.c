#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "particula.h"
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

    int i;

    // Creacion de los pipes
    for (i = 0; i < workers; i++)
    {
        if (pipe(fdWorkerToBroken[i]) == -1 || pipe(fdBrokenToWorker[i]) == -1)
        {
            printf("Error al crear un pipe");
            exit(0);
        }
    }

    // Creacion de los workers
    for (i = 0; i < workers; i++)
    {
        int pid = fork();

        if (pid == 0)
        {                                                // worker
            close(fdWorkerToBroken[i][0]);               // Cerramos el pipe de lectura, ya que este pipe se usa para que el worker escriba y broker lea
            close(fdBrokenToWorker[i][1]);               // Cerramos el pipe de escritura, ya que este pipe se usa para que el worker lea y broker escriba

            dup2(fdWorkerToBroken[i][1], STDOUT_FILENO); // Redireccionamos la salida estandar al pipe de escritura
            dup2(fdBrokenToWorker[i][0], STDIN_FILENO);  // Redireccionamos la entrada estandar al pipe de lectura
            
            char *args[] = {"./worker", argv[1], NULL};
            execv(args[0], args);
        }
        else
        {                                  // broker
            close(fdWorkerToBroken[i][1]); // Cerramos el pipe de escritura, ya que este pipe se usa para que el broker lea y worker escriba
            close(fdBrokenToWorker[i][0]); // Cerramos el pipe de lectura, ya que este pipe se usa para que el broker escriba y worker lea
        }
    }

    FILE *archivoParticulas = fopen(input, "r");
    if (archivoParticulas == NULL)
    {
        printf("Error al abrir el archivo de entrada\n");
        exit(0);
    }

    int cantidadParticulas;
    fscanf(archivoParticulas, "%d", &cantidadParticulas);

    int lineasLeidas = 0;
    char buffer[50];
    int workerRandom;
    srand(time(NULL));
    // Cuando se la cantidad de lineas alcance o pase a la cantidad de particulas, se envia el mensaje 'FIN' a los workers
    fgets(buffer, sizeof(buffer), archivoParticulas);
    while (lineasLeidas < cantidadParticulas)
    {
        if ((lineasLeidas % chunks) == 0) // cuando ya se han leido 'chunks' lineas, se cambia a otro worker de forma aleatoria
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

    // Lectura de los resultados de los workers  (BOSQUEJO)
    double *arregloCeldas = (double *)malloc(celdas * sizeof(double));
    char archivoWorker[50];
    wait(NULL);
    for (i = 0; i < workers; i++)
    {
        int lineasTrabajadas;
        read(fdWorkerToBroken[i][PIPE_READ], &lineasTrabajadas, sizeof(int));
        read(fdWorkerToBroken[i][PIPE_READ], archivoWorker, sizeof(archivoWorker));
        printf("Worker %d: %d lineas trabajadas\n", i+1, lineasTrabajadas);
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

    int j;
    if (D)
    {
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

    free(arregloCeldas);
    return 0;
}