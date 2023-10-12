#include "funciones.h"

// Entradas: Nombre del archivo de entrada (char *NameFile), Puntero a la cantidad de particulas (int *cantidad_Particulas)
// Salidas: Un array de estructuras "Particula", cada estructura contiene la celda de impacto y su energía.
// Descripción: Lee un archivo de entrada que contiene información sobre partículas,
//              crea un array de estructuras Particula y lo llena con los datos del archivo.
//              Devuelve el array generado y almacena la cantidad de partículas en la variable cantidad_Particulas (en el puntero proporcionado).
Particula *lecturaParticulas(char *NameFile, int *cantidad_Particulas)
{
    int i;

    FILE *particulasFile = fopen(NameFile, "r");
    fscanf(particulasFile, "%d", cantidad_Particulas); // Obtenemos la cantidad de particulas presentes ene le archivo

    Particula *Array_Particulas = (Particula *)malloc(sizeof(Particula) * (*cantidad_Particulas)); // Reservamos memoria para guardar la informacion de todas las particulas

    for (i = 0; i < *cantidad_Particulas; i++) // Recorremos el archivo, guardando las celdas de impacto y su energia en las estructuras Particulas
    {
        fscanf(particulasFile, "%d %lf", &Array_Particulas[i].posicion, &Array_Particulas[i].energia);
    }

    fclose(particulasFile);
    return Array_Particulas;
}

// Entradas: Número de celdas (int celdas), Energía inicial (double energia_Inicial), Posición de la celda (int posicion_Celda),
//           Energía de la partícula (double energia_Particula), Posición de la partícula (int posicion_Particula)
// Salidas: Energía acumulada en la celda (double)
// Descripción: Calcula la energía repartida en una celda dada de acuerdo a una fórmula específica.
//              Compara la energía repartida con un umbral y, si es mayor, la suma a la energía inicial de la celda.
//              Devuelve la energía acumulada en la celda.
double formulaYComparacion(int celdas, double energia_Inicial, int posicion_Celda, double energia_Particula, int posicion_Particula)
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

// Entradas: Array de celdas (double *array_Celdas), cantidad de celdas (int cantidad_Celdas)
// Salidas: Ninguna
// Descripción: Muestra información sobre las celdas y la energía acumulada en cada una en un formato de salida específico.
void mostrarCeldas(double *array_Celdas, int cantidad_Celdas)
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

// Entradas: Array de celdas (double *array_Celdas), cantidad de celdas (int cantidad_Celdas), nombre del archivo (char *nombreArchivo)
// Salidas: Ninguna
// Descripción: Escribe información sobre las celdas y la celda con la energía máxima en un archivo de salida.
void escrituraArchivo(double *array_Celdas, int cantidad_Celdas, char *nombreArchivo)
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

// Entradas: Array de celdas (double *array_Celdas), cantidad de celdas (int cantidad_Celdas), puntero al valor máximo (MaxValor), puntero a la posición del valor máximo (posicionMaxValor)
// Salidas: Ninguna
// Descripción: Encuentra la celda con la energía máxima y almacena el valor máximo en MaxValor y su posición en posicionMaxValor.
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