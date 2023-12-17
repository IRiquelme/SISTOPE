#include "funciones.h"

// Entradas: Número de celdas (int celdas), Energía inicial (double energia_Inicial), Posición de la celda (int posicion_Celda),
//           Energía de la partícula (double energia_Particula), Posición de la partícula (int posicion_Particula)
// Salidas: Energía acumulada en la celda (double)
// Descripción: Calcula la energía repartida en una celda dada de acuerdo a una fórmula específica.
//              Compara la energía repartida con un umbral y, si es mayor, la suma a la energía inicial de la celda.
//              Finalmente, devuelve la energía acumulada en la celda.
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

// Entradas: Argumentos de la hebra (void *args)
// Salidas: Ninguna
// Descripción: Lee partículas del archivo y actualiza la energía de las celdas correspondientes.
void *procesar_archivo(void *args)
{
    ArgumentosHebra *argumentos = (ArgumentosHebra *)args;
    int particulasEnBuffer;

    Particula *bufferParticulas = (Particula *)malloc(sizeof(Particula) * argumentos->chunk);

    while (1)
    {
        particulasEnBuffer = 0;

        pthread_mutex_lock(&argumentos->semaforos[0]); // EnterCS()

        if (argumentos->flags[1]) // Si el archivo ya termino, entonces se sale del while
        {
            pthread_mutex_unlock(&argumentos->semaforos[0]); // ExitCS()
            break;
        }

        leer_archivo(argumentos, bufferParticulas, &particulasEnBuffer);

        pthread_mutex_unlock(&argumentos->semaforos[0]); // ExitCS()

        if (particulasEnBuffer > 0)
        {
            pthread_mutex_lock(&argumentos->semaforos[1]); // EnterCS()

            actualizar_energia(particulasEnBuffer, bufferParticulas, argumentos);

            pthread_mutex_unlock(&argumentos->semaforos[1]); // ExitCS()
        }
    }
    return NULL;
}

// Entradas: Número de hebras (int hebras), Número total de celdas (int numeroCeldas),
//           Arreglo de líneas trabajadas por cada hebra (int *lineasTrabajasPorWorker),
//           Arreglo de energías en cada celda (double *arregloCeldas)
// Salidas: Ninguna
// Descripción: Muestra información sobre el trabajo realizado por cada hebra y la energía en cada celda.
void mostrar_resultado(int hebras, int numeroCeldas, int *lineasTrabajasPorWorker, double *arregloCeldas)
{
    int i, j;
    for (i = 0; i < hebras; i++)
    {
        printf("Hebra %d: %d lineas procesadas\n", i + 1, lineasTrabajasPorWorker[i]);
    }

    for (i = 0; i < numeroCeldas; i++)
    {
        printf("%d %lf |", i, arregloCeldas[i]);
        for (j = 0; j < arregloCeldas[i] / 70; j++)
        {
            printf("o");
        }
        printf("\n");
    }
}

// Entradas: Número de hebras (int hebras), Número total de celdas (int celdas), Numero de lineas a leer por ciclo (int chunk),
//           Nombre del archivo (char *nombreArchivo), Arreglo de energías en cada celda (double *arregloCeldas),
//           Arreglo de líneas trabajadas por cada hebra (int *lineasTrabajadas)
// Salidas: Ninguna
// Descripción: Crea y despliega hebras para procesar el archivo. Luego espera a que todas las hebras terminen su trabajo
void crear_desplegar_hebras(int hebras, int celdas, int chunk, char *nombreArchivo, double *arregloCeldas, int *lineasTrabajadas)
{
    pthread_t *arregloHebras = (pthread_t *)malloc(sizeof(pthread_t) * hebras);
    ArgumentosHebra *arregloArgumentos = (ArgumentosHebra *)malloc(sizeof(ArgumentosHebra) * hebras);
    pthread_mutex_t *semaforos = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * 2);
    int *flags = (int *)calloc(2, sizeof(int));

    flags[0] = 1; // flagPrimeraLinea
    flags[1] = 0; // flagFinalArchivo
    pthread_mutex_init(&semaforos[0], NULL);
    pthread_mutex_init(&semaforos[1], NULL);

    FILE *archivo = fopen(nombreArchivo, "r");

    for (int i = 0; i < hebras; i++)
    {
        arregloArgumentos[i].idHebra = i;
        arregloArgumentos[i].arregloCeldas = arregloCeldas;
        arregloArgumentos[i].lineasTrabajadas = lineasTrabajadas;
        arregloArgumentos[i].flags = flags;
        arregloArgumentos[i].chunk = chunk;
        arregloArgumentos[i].largoCeldas = celdas;
        arregloArgumentos[i].archivo = archivo;
        arregloArgumentos[i].semaforos = semaforos;

        pthread_create(&arregloHebras[i], NULL, procesar_archivo, (void *)&arregloArgumentos[i]);
    }

    for (int i = 0; i < hebras; i++)
    {
        pthread_join(arregloHebras[i], NULL);
    }

    fclose(archivo);
    free(arregloHebras);
    free(arregloArgumentos);
    free(flags);
    free(semaforos);
}

// Entradas: Arreglo de celdas (double *arregloCeldas), número total de celdas (int celdas)
// Salida: Índice de la celda con la máxima energía (int)
// Descripción: Encuentra y devuelve el índice de la celda con la máxima energía en el arreglo de celdas proporcionado.
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

// Entradas: Arreglo de celdas (double *arregloCeldas), número total de celdas (int celdas), y nombre del archivo de salida (const char *output)
// Salida: Ninguna
// Descripción: Crea un archivo de salida con el nombre proporcionado. Escribe en el archivo la información sobre la celda con la máxima energía y los valores de energía de todas las celdas.
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

// Entradas: Número de partículas en el buffer (int particulasEnBuffer), Buffer de partículas (Particula *bufferParticulas), Argumentos de la hebra (ArgumentosHebra *argumentos)
// Salidas: Ninguna
// Descripción: Actualiza la energía en el arreglo de celdas según las partículas en el buffer.
void actualizar_energia(int particulasEnBuffer, Particula *bufferParticulas, ArgumentosHebra *argumentos)
{
    int i, j;
    for (i = 0; i < particulasEnBuffer; i++)
    {
        for (j = 0; j < argumentos->largoCeldas; j++)
        {
            argumentos->arregloCeldas[j] = formula(argumentos->largoCeldas, argumentos->arregloCeldas[j], j, bufferParticulas[i].energia, bufferParticulas[i].posicion);
        }
        argumentos->lineasTrabajadas[argumentos->idHebra]++;
    }
}

// Entradas: Argumentos de la hebra (ArgumentosHebra *argumentos), Buffer de partículas (Particula *bufferParticulas), Puntero a la variable que contabiliza la cantidad de particulas en el buffer (int *particulasEnBuffer)
// Salidas: Ninguna
// Descripción: Lee partículas 'chunk' lineas desde el archivo y las almacena en un buffer de particulas.
void leer_archivo(ArgumentosHebra *argumentos, Particula *bufferParticulas, int *particulasEnBuffer)
{
    int i;

    if (argumentos->flags[0])
    { // Si es la primera linea del archivo, se cambia el flag a 0 py se descarta la linea ya que no se utiliza
        argumentos->flags[0] = 0;
        fscanf(argumentos->archivo, "%*[^\n]\n");
    }

    for (i = 0; i < argumentos->chunk; i++)
    {
        if (fscanf(argumentos->archivo, "%d %lf", &bufferParticulas[i].posicion, &bufferParticulas[i].energia) == EOF)
        {
            argumentos->flags[1] = 1; // Si se llega al final del archivo, se cambia el flag a 1 para salir del while en la proxima iteracion
            break;
        }
        (*particulasEnBuffer)++;
    }
}