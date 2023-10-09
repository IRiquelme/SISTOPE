#include "funciones.h"

// Entradas:
// Salidas:
// Descripcion:
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
        fscanf(particulasFile, "%d %lf", &Array_Particulas[i].posicion, &Array_Particulas[i].energia);
    }

    fclose(particulasFile);
    return Array_Particulas;
}

// Entradas:
// Salidas:
// Descripcion:
double FormulaYComparacion(int celdas, double energia_Inicial, int posicion_Celda, double energia_Particula, int posicion_Particula)
{
    double energia_Repartida;
    double min_energia;

    energia_Repartida = (pow(10.0, 3.0) * energia_Particula) / (celdas * (double)(sqrt(abs(posicion_Particula - posicion_Celda) + 1.0)));
    min_energia = pow(10.0, -3.0) / celdas;

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

// Entradas:
// Salidas:
// Descripcion:
void MostrarCeldas(double *array_Celdas, int cantidad_Celdas)
{
    int i, j;
    for (i = 0; i < cantidad_Celdas; i++)
    {
        printf("%d %.6f |", i, array_Celdas[i]);
        for (j = 0; j < array_Celdas[i] / 50; j++)
        {
            printf("o");
        }
        printf("\n");
    }
}

// Entradas:
// Salidas:
// Descripcion:
void EscrituraArchivo(double *array_Celdas, int cantidad_Celdas, char *nombreArchivo)
{
    FILE *archivo;
    archivo = fopen(nombreArchivo, "w");

    double EnergiaMax;
    int CeldaMax;

    MaxEnergiaCelda(array_Celdas, cantidad_Celdas, &EnergiaMax, &CeldaMax);

    fprintf(archivo, "%d %.6f\n", CeldaMax, EnergiaMax);

    int i;

    for (i = 0; i < cantidad_Celdas; i++)
    {
        fprintf(archivo, "%d %.6f\n", i, array_Celdas[i]);
    }
    fclose(archivo);
}

// Entradas:
// Salidas:
// Descripcion:
void MaxEnergiaCelda(double *array_Celdas, int cantidad_Celdas, double *MaxValor, int *posicionMaxValor)
{
    double max = array_Celdas[0];
    *posicionMaxValor = 0;

    int i;
    for (i = 0; i < cantidad_Celdas; i++)
    {
        if (array_Celdas[i] > max)
        {
            max = array_Celdas[i];
            *posicionMaxValor = i;
        }
    }
    *MaxValor = max;
}