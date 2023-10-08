#ifndef FUNCIONES_H
#define FUNCIONES_H

Particulas *Lectura_Particulas(char *NameFile);
int FormulaYComparacion(int celdas, float energia_Inicial, int posicion_Celda, float energia_Particula, int posicion_Particula);
void MostrarCeldas(float *array_Celdas, int cantidad_Celdas);

#endif