#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "particula.h"

// Entradas:
// Salidas:
// Descripcion:
Particulas *Lectura_Particulas(char *NameFile);

// Entradas:
// Salidas:
// Descripcion:
double FormulaYComparacion(int celdas, double energia_Inicial, int posicion_Celda, double energia_Particula, int posicion_Particula);

// Entradas:
// Salidas:
// Descripcion:
void MostrarCeldas(double *array_Celdas, int cantidad_Celdas);

// Entradas:
// Salidas:
// Descripcion:
void EscrituraArchivo(double *array_Celdas, int cantidad_Celdas, char *nombreArchivo);

// Entradas:
// Salidas:
// Descripcion:
void MaxEnergiaCelda(double *array_Celdas, int cantidad_Celdas, double *MaxValor, int *posicionMaxValor);