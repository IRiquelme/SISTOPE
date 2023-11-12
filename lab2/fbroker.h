#ifndef FBROKER_H
#define FBROKER_H

//Entradas: Número de workers (int workers), arreglos de pipes de worker a broken (int fdWorkerToBroken[][2]) y de broken a worker (int fdBrokenToWorker[][2])
//Salida: Ninguna
//Descripción: Cierra los extremos no utilizados de los pipes para la comunicación entre workers y broken. 
//             Recibe la cantidad de workers y los arreglos de pipes y cierra los extremos de lectura ([0]) de los pipes fdWorkerToBroken 
//             y los extremos de escritura ([1]) de los pipes fdBrokenToWorker para cada worker.
void cerrar_pipes(int workers, int fdWorkerToBroken[][2], int fdBrokenToWorker[][2]);

//Entradas: Número de workers (int workers), arreglos de pipes de worker a broken (int fdWorkerToBroken[][2]) y de broken a worker (int fdBrokenToWorker[][2]).
//Salida: Ninguna
//Descripción: Crea pipes para la comunicación bidireccional entre workers y broken. 
//             Utiliza la función pipe para crear un par de pipes (uno para cada dirección de comunicación) para cada worker.
void crear_pipes(int workers, int fdWorkerToBroken[][2], int fdBrokenToWorker[][2]);

//Entradas: Número de workers (int workers), arreglos de pipes de worker a broken (int fdWorkerToBroken[][2]),
//          de broken a worker (int fdBrokenToWorker[][2]), y el arreglo de argumentos del programa (char *argv[])
//Salida: Ninguna
//Descripción: Crea un número específico de procesos hijos (workers) utilizando la llamada al sistema fork. 
//             Cada worker tiene su propia comunicación bidireccional a través de pipes con el proceso padre (broken). 
//             Redirecciona la entrada y salida estándar de los workers a través de los pipes utilizando dup2, 
//             y ejecuta el programa de worker con los argumentos proporcionados en argv.
void crear_workers(int workers, int fdWorkerToBroken[][2], int fdBrokenToWorker[][2], char *argv[]);

//Entradas: Número total de celdas (int celdas), número de workers (int workers), arreglo de celdas (double *arregloCeldas),
//          arreglos de pipes de worker a broken (int fdWorkerToBroken[][2]), y arreglo para almacenar las líneas trabajadas por cada worker (int *lineasTrabajadasPorWorker)
//Salida: Ninguna
//Descripción: Procesa las respuestas enviadas por los workers a través de los pipes. 
//             Lee el número de líneas trabajadas y el nombre del archivo de salida de cada worker. 
//             Abre y lee el archivo, actualizando el arreglo de celdas con los valores correspondientes. 
//             Elimina los archivos temporales creados por los workers.
void procesar_respuesta_workers(int celdas, int workers, double *arregloCeldas, int fdWorkerToBroken[][2], int *lineasTrabajadasPorWorker);

//Entradas: Arreglo de líneas trabajadas por cada worker (int *lineasTrabajadasPorWorker), arreglo de celdas (double *arregloCeldas),
//          número total de celdas (int celdas), y número de workers (int workers)
//Salida: Ninguna
//Descripción: Muestra en la consola la cantidad de líneas procesadas por cada worker y representa gráficamente los valores de las celdas en función de su energía, 
//             utilizando un símbolo 'o' para cada incremento de 50 unidades de energía.
void mostrar_resultado(int *lineasTrabajasPorWorker, double *arregloCeldas, int celdas, int workers);

//Entradas: Nombre del archivo de entrada (const char *input), cantidad de chunks para asignar trabajo (int chunks), 
//          número de workers (int workers), y arreglo de pipes de broken a worker (int fdBrokenToWorker[][2])
//Salida: Ninguna
//Descripción: Asigna líneas de trabajo desde un archivo de entrada a los workers utilizando pipes. 
//             Lee las líneas del archivo y las escribe en el pipe correspondiente a un worker seleccionado aleatoriamente. 
//             Cuando se han asignado todas las líneas, envía la señal de fin ("FIN") a todos los workers.
void asignar_trabajo(const char *input, int chunks, int workers, int fdBrokenToWorker[][2]);

//Entradas: Arreglo de celdas (double *arregloCeldas), número total de celdas (int celdas)
//Salida: Índice de la celda con la máxima energía (int)
//Descripción: Encuentra y devuelve el índice de la celda con la máxima energía en el arreglo de celdas proporcionado.
int obtener_maximo(double *arregloCeldas, int celdas);

//Entradas: Arreglo de celdas (double *arregloCeldas), número total de celdas (int celdas), y nombre del archivo de salida (const char *output)
//Salida: Ninguna
//Descripción: Crea un archivo de salida con el nombre proporcionado. 
//             Escribe en el archivo la información sobre la celda con la máxima energía y los valores de energía de todas las celdas.
void crear_archivo_salida(double *arregloCeldas, int celdas, const char *output);

#endif