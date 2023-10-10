#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "particula.h"

// Entradas:
// Salidas:
// Descripcion:
Particula *lecturaParticulas(char *NameFile, int *cantidad_Particulas);

// Entradas:
// Salidas:
// Descripcion:
double formulaYComparacion(int celdas, double energia_Inicial, int posicion_Celda, double energia_Particula, int posicion_Particula);

// Entradas:
// Salidas:
// Descripcion:
void mostrarCeldas(double *array_Celdas, int cantidad_Celdas);

// Entradas:
// Salidas:
// Descripcion:
void escrituraArchivo(double *array_Celdas, int cantidad_Celdas, char *nombreArchivo);

// Entradas:
// Salidas:
// Descripcion:
void MaxEnergiaCelda(double *array_Celdas, int cantidad_Celdas, double *MaxValor, int *posicionMaxValor);