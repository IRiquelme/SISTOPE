#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "argumentosHebra.h"
#include "particula.h"


// Entradas: Número de celdas (int celdas), Energía inicial (double energia_Inicial), Posición de la celda (int posicion_Celda),
//           Energía de la partícula (double energia_Particula), Posición de la partícula (int posicion_Particula)
// Salidas: Energía acumulada en la celda (double)
// Descripción: Calcula la energía repartida en una celda dada de acuerdo a una fórmula específica.
//              Compara la energía repartida con un umbral y, si es mayor, la suma a la energía inicial de la celda.
//              Finalmente, devuelve la energía acumulada en la celda.
double formula(int celdas, double energiaInicial, int posicionCelda, double energiaParticula, int posicionParticula);

// Entradas: Argumentos de la hebra (void *args)
// Salidas: Ninguna
// Descripción: Lee partículas del archivo y actualiza la energía de las celdas correspondientes.
void *procesar_archivo(void *args);

// Entradas: Número de hebras (int hebras), Número total de celdas (int numeroCeldas),
//           Arreglo de líneas trabajadas por cada hebra (int *lineasTrabajasPorWorker),
//           Arreglo de energías en cada celda (double *arregloCeldas)
// Salidas: Ninguna
// Descripción: Muestra información sobre el trabajo realizado por cada hebra y la energía en cada celda.
void mostrar_resultado(int hebras, int numeroCeldas, int *lineasTrabajasPorWorker, double *arregloCeldas);

// Entradas: Número de hebras (int hebras), Número total de celdas (int celdas), Numero de lineas a leer por ciclo (int chunk),
//           Nombre del archivo (char *nombreArchivo), Arreglo de energías en cada celda (double *arregloCeldas),
//           Arreglo de líneas trabajadas por cada hebra (int *lineasTrabajadas)
// Salidas: Ninguna
// Descripción: Crea y despliega hebras para procesar el archivo. Luego espera a que todas las hebras terminen su trabajo
void crear_desplegar_hebras(int hebras, int celdas, int chunk, char *nombreArchivo, double* arregloCeldas, int* lineasTrabajadas);

// Entradas: Arreglo de celdas (double *arregloCeldas), número total de celdas (int celdas)
// Salida: Índice de la celda con la máxima energía (int)
// Descripción: Encuentra y devuelve el índice de la celda con la máxima energía en el arreglo de celdas proporcionado.
void crear_archivo_salida(double *arregloCeldas, int celdas, const char *output);

// Entradas: Arreglo de celdas (double *arregloCeldas), número total de celdas (int celdas), y nombre del archivo de salida (const char *output)
// Salida: Ninguna
// Descripción: Crea un archivo de salida con el nombre proporcionado. Escribe en el archivo la información sobre la celda con la máxima energía y los valores de energía de todas las celdas.
int obtener_maximo(double *arregloCeldas, int celdas);

// Entradas: Número de partículas en el buffer (int particulasEnBuffer), Buffer de partículas (Particula *bufferParticulas), Argumentos de la hebra (ArgumentosHebra *argumentos)
// Salidas: Ninguna
// Descripción: Actualiza la energía en el arreglo de celdas según las partículas en el buffer.
void actualizar_energia(int particulasEnBuffer,Particula* bufferParticulas, ArgumentosHebra* argumentos);

// Entradas: Argumentos de la hebra (ArgumentosHebra *argumentos), Buffer de partículas (Particula *bufferParticulas), Puntero a la variable que contabiliza la cantidad de particulas en el buffer (int *particulasEnBuffer)
// Salidas: Ninguna
// Descripción: Lee partículas 'chunk' lineas desde el archivo y las almacena en un buffer de particulas.
void leer_archivo(ArgumentosHebra *argumentos, Particula *bufferParticulas, int *particulasEnBuffer);