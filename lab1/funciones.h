#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "particula.h"

// Entradas: Nombre del archivo de entrada (char *NameFile), Puntero a la cantidad de particulas (int *cantidad_Particulas)
// Salidas: Un array de estructuras "Particula", cada estructura contiene la celda de impacto y su energía.
// Descripción: Lee un archivo de entrada que contiene información sobre partículas,
//              crea un array de estructuras Particula y lo llena con los datos del archivo.
//              Devuelve el array generado y almacena la cantidad de partículas en la variable cantidad_Particulas (en el puntero proporcionado).
Particula *lecturaParticulas(char *NameFile, int *cantidad_Particulas);

// Entradas: Número de celdas (int celdas), Energía inicial (double energia_Inicial), Posición de la celda (int posicion_Celda),
//           Energía de la partícula (double energia_Particula), Posición de la partícula (int posicion_Particula)
// Salidas: Energía acumulada en la celda (double)
// Descripción: Calcula la energía repartida en una celda dada de acuerdo a una fórmula específica.
//              Compara la energía repartida con un umbral y, si es mayor, la suma a la energía inicial de la celda.
//              Devuelve la energía acumulada en la celda.
double formulaYComparacion(int celdas, double energia_Inicial, int posicion_Celda, double energia_Particula, int posicion_Particula);

// Entradas: Array de celdas (double *array_Celdas), cantidad de celdas (int cantidad_Celdas)
// Salidas: Ninguna
// Descripción: Muestra información sobre las celdas y la energía acumulada en cada una en un formato de salida específico.
void mostrarCeldas(double *array_Celdas, int cantidad_Celdas);

// Entradas: Array de celdas (double *array_Celdas), cantidad de celdas (int cantidad_Celdas), nombre del archivo (char *nombreArchivo)
// Salidas: Ninguna
// Descripción: Escribe información sobre las celdas y la celda con la energía máxima en un archivo de salida.
void escrituraArchivo(double *array_Celdas, int cantidad_Celdas, char *nombreArchivo);

// Entradas: Array de celdas (double *array_Celdas), cantidad de celdas (int cantidad_Celdas), puntero al valor máximo (MaxValor), puntero a la posición del valor máximo (posicionMaxValor)
// Salidas: Ninguna
// Descripción: Encuentra la celda con la energía máxima y almacena el valor máximo en MaxValor y su posición en posicionMaxValor.
void MaxEnergiaCelda(double *array_Celdas, int cantidad_Celdas, double *MaxValor, int *posicionMaxValor);