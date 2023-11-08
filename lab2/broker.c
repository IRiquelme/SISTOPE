#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "particula.h"

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

    int fdBroker[workers][2]; // Matriz para los pipes con mensajes para el broker (Broker lee; Workers escriben)
    int fdWorker[workers][2]; // Matriz para los pipes con mensajes para el worker (Worker lee; Broker escribe)

    int i;

    // Creacion de los pipes
    for (i = 0; i < workers; i++)
    {
        if (pipe(fdBroker[i]) == -1 || pipe(fdWorker[i]) == -1)
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
        {   // worker
            close(fdBroker[i][0]); // Cerramos el pipe de lectura, ya que este pipe se usa para que el worker escriba y broker lea
            close(fdWorker[i][1]); // Cerramos el pipe de escritura, ya que este pipe se usa para que el worker lea y broker escriba
            dup2(fdBroker[i][1], STDOUT_FILENO); // Redireccionamos la salida estandar al pipe de escritura
            dup2(fdWorker[i][0], STDIN_FILENO); // Redireccionamos la entrada estandar al pipe de lectura
            execv("./worker", NULL);    // Quizas hay que mandar el argumento 'chunks', para que el worker sepa con cuantas lineas trabajara (creacion de arreglo). Incluso tambien 'celdas'
        }
        else
        {   // broker
            close(fdBroker[i][1]); // Cerramos el pipe de escritura, ya que este pipe se usa para que el broker lea y worker escriba
            close(fdWorker[i][0]); // Cerramos el pipe de lectura, ya que este pipe se usa para que el broker escriba y worker lea
        }
    }
    
    FILE *archivoParticulas = fopen(input, "r");
    if (archivoParticulas == NULL)
    {
        printf("Error al abrir el archivo de entrada");
        exit(0);
    }
    int cantidadParticulas;
    fscanf(archivoParticulas, "%d", cantidadParticulas);    // Cuando se la cantidad de lineas alcance o pase a la cantidad de particulas, se envia el mensaje 'FIN' a los workers
    
    // BOSQUEJO DE COMO SE LEERA EL ARCHIVO DE PARTICULAS Y SE ENVIARA A LOS WORKERS
    int lineasLeidas = 0;
    char chunkLeido [chunks][50];
    while (lineasLeidas < cantidadParticulas)
    {
        for (i = 0; i < chunks; i++)
        {
            fscanf(archivoParticulas, "%s", chunkLeido[i]);
            lineasLeidas++;
        }
        int workerRandom = rand() % workers;
        write(fdWorker[workerRandom][1], chunkLeido, sizeof(chunkLeido));
    }

    return 0;
}