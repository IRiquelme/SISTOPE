#include "fworker.h"

// Entradas: Número de celdas (int celdas), Energía inicial (double energia_Inicial), Posición de la celda (int posicion_Celda),
//           Energía de la partícula (double energia_Particula), Posición de la partícula (int posicion_Particula)
// Salidas: Energía acumulada en la celda (double)
// Descripción: Calcula la energía repartida en una celda dada de acuerdo a una fórmula específica.
//              Compara la energía repartida con un umbral y, si es mayor, la suma a la energía inicial de la celda.
//              Devuelve la energía acumulada en la celda.
double formula(int celdas, double energiaInicial, int posicionCelda, double energiaParticula, int posicionParticula)
{
    double energiaRepartida;
    double energiaMinima;

    energiaRepartida = (pow(10.0, 3.0) * energiaParticula) / (celdas * (double)(sqrt(abs(posicionParticula - posicionCelda) + 1.0)));
    energiaMinima = pow(10.0, -3.0) / celdas;

    // Si la energia repartida es mayor a la energia minima, entonces si se le suma a la celda y si no, entonces se retorna la energia inicial
    if (energiaRepartida >= energiaMinima)
    {
        return energiaInicial + energiaRepartida;
    }
    else
    {
        return energiaInicial;
    }
}
//Entradas: Arreglo de celdas(double * arreglo), Cantidad de celdas (int) y Nombre del archivo (char *fileName)
//Salida: Ninguna
//Descripcion: Genera un archivo, en donde se escribe información sobre las celdas y la celda con la energía máxima en un archivo de salida.
void generar_archivo_resultado(double *arreglo, int celdas, char *fileName)
{
    FILE *fp = fopen(fileName, "a");
    int i;
    for (i = 0; i < celdas; i++)
    {
        fprintf(fp, "%lf\n", arreglo[i]);
    }
    fclose(fp);
}