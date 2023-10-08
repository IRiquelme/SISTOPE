#include "funciones.h"

Particulas *Lectura_Particulas(char *NameFile)
{
    int i;
    int Cantidad_Particulas;

    FILE *particulasFile = fopen(NameFile, "r");
    fscanf(particulasFile, "%d", &Cantidad_Particulas);

    Particulas *Array_Particulas = (Particulas *)malloc(sizeof(Particulas) * Cantidad_Particulas);

    for (i = 0; i < Cantidad_Particulas; i++)
    {
        Array_Particulas[i].c_Particulas = Cantidad_Particulas;
        fscanf(particulasFile, "%d %f", &Array_Particulas[i].posicion, &Array_Particulas[i].energia);
    }

    fclose(particulasFile);
    return Array_Particulas;
}

int FormulaYComparacion(int celdas, float energia_Inicial, int posicion_Celda, float energia_Particula, int posicion_Particula)
{
    float energia_Repartida;
    float min_energia;

    energia_Repartida = (pow(10, 3) * energia_Particula) / (celdas * (sqrt(abs(posicion_Particula - posicion_Celda) + 1)));
    min_energia = pow(10, -3) / celdas;

    // Si la energia repartida es mayor a la energia minima, entonces si se le suma a la celda y si no, entonces se retorna la energia inicial
    if (min_energia <= energia_Repartida)
    {
        return energia_Inicial + energia_Repartida;
    }
    else
    {
        return energia_Inicial;
    }
}

void MostrarCeldas(float *array_Celdas, int cantidad_Celdas)
{
    int i;
    for (i = 0; i < cantidad_Celdas; i++)
    {
        printf("Celda: %d, Energia: %0.6f \n", i, array_Celdas[i]);
    }
}