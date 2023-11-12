#include "fbroker.h"
#include <time.h>
#define PIPE_READ 0
#define PIPE_WRITE 1

//Entradas: Número de workers (int workers), arreglos de pipes de worker a broken (int fdWorkerToBroken[][2]) y de broken a worker (int fdBrokenToWorker[][2])
//Salida: Ninguna
//Descripción: Cierra los extremos no utilizados de los pipes para la comunicación entre workers y broken. 
//             Recibe la cantidad de workers y los arreglos de pipes y cierra los extremos de lectura ([0]) de los pipes fdWorkerToBroken 
//             y los extremos de escritura ([1]) de los pipes fdBrokenToWorker para cada worker.
void cerrar_pipes(int workers, int fdWorkerToBroken[][2], int fdBrokenToWorker[][2])
{
    int i;
    for (i = 0; i < workers; i++)
    {
        close(fdWorkerToBroken[i][0]);
        close(fdBrokenToWorker[i][1]);
    }
}

//Entradas: Número de workers (int workers), arreglos de pipes de worker a broken (int fdWorkerToBroken[][2]) y de broken a worker (int fdBrokenToWorker[][2]).
//Salida: Ninguna
//Descripción: Crea pipes para la comunicación bidireccional entre workers y broken. 
//             Utiliza la función pipe para crear un par de pipes (uno para cada dirección de comunicación) para cada worker.
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

//Entradas: Número de workers (int workers), arreglos de pipes de worker a broken (int fdWorkerToBroken[][2]),
//          de broken a worker (int fdBrokenToWorker[][2]), y el arreglo de argumentos del programa (char *argv[])
//Salida: Ninguna
//Descripción: Crea un número específico de procesos hijos (workers) utilizando la llamada al sistema fork. 
//             Cada worker tiene su propia comunicación bidireccional a través de pipes con el proceso padre (broken). 
//             Redirecciona la entrada y salida estándar de los workers a través de los pipes utilizando dup2, 
//             y ejecuta el programa de worker con los argumentos proporcionados en argv.
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

//Entradas: Número total de celdas (int celdas), número de workers (int workers), arreglo de celdas (double *arregloCeldas),
//          arreglos de pipes de worker a broken (int fdWorkerToBroken[][2]), y arreglo para almacenar las líneas trabajadas por cada worker (int *lineasTrabajadasPorWorker)
//Salida: Ninguna
//Descripción: Procesa las respuestas enviadas por los workers a través de los pipes. 
//             Lee el número de líneas trabajadas y el nombre del archivo de salida de cada worker. 
//             Abre y lee el archivo, actualizando el arreglo de celdas con los valores correspondientes. 
//             Elimina los archivos temporales creados por los workers.
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

//Entradas: Arreglo de líneas trabajadas por cada worker (int *lineasTrabajadasPorWorker), arreglo de celdas (double *arregloCeldas),
//          número total de celdas (int celdas), y número de workers (int workers)
//Salida: Ninguna
//Descripción: Muestra en la consola la cantidad de líneas procesadas por cada worker y representa gráficamente los valores de las celdas en función de su energía, 
//             utilizando un símbolo 'o' para cada incremento de 50 unidades de energía.
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

//Entradas: Nombre del archivo de entrada (const char *input), cantidad de chunks para asignar trabajo (int chunks), 
//          número de workers (int workers), y arreglo de pipes de broken a worker (int fdBrokenToWorker[][2])
//Salida: Ninguna
//Descripción: Asigna líneas de trabajo desde un archivo de entrada a los workers utilizando pipes. 
//             Lee las líneas del archivo y las escribe en el pipe correspondiente a un worker seleccionado aleatoriamente. 
//             Cuando se han asignado todas las líneas, envía la señal de fin ("FIN") a todos los workers.
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

//Entradas: Arreglo de celdas (double *arregloCeldas), número total de celdas (int celdas)
//Salida: Índice de la celda con la máxima energía (int)
//Descripción: Encuentra y devuelve el índice de la celda con la máxima energía en el arreglo de celdas proporcionado.
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

//Entradas: Arreglo de celdas (double *arregloCeldas), número total de celdas (int celdas), y nombre del archivo de salida (const char *output)
//Salida: Ninguna
//Descripción: Crea un archivo de salida con el nombre proporcionado. 
//             Escribe en el archivo la información sobre la celda con la máxima energía y los valores de energía de todas las celdas.
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