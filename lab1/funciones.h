#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "particula.h"

Particulas *Lectura_Particulas(char *NameFile);
float FormulaYComparacion(int celdas, float energia_Inicial, int posicion_Celda, float energia_Particula, int posicion_Particula);
void MostrarCeldas(float *array_Celdas, int cantidad_Celdas);
void EscrituraArchivo(float * array_Celdas, int cantidad_Celdas, char * nombreArchivo);
void MaxEnergiaCelda (float * array_Celdas, int cantidad_Celdas, float *MaxValor, int * posicionMaxValor);