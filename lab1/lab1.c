#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"
#include "particula.h"

int main()
{
	char nombreInput[20] = "test1_35.txt"; // Nombre o variable donde el archivo de entrada de particulas se llama
	Particulas *Lista_Particulas = Lectura_Particulas(nombreInput);

	int cantidad_celdas = 35; // Nombre o variable donde se estarian almacenando las celdas que se establecen del 1 al 6

	float *Array_Celdas = (float *)calloc(cantidad_celdas, sizeof(float)); // Inicializar el array con solo 0

	printf("Antes del impacto de las particulas\n");
	MostrarCeldas(Array_Celdas, cantidad_celdas);

	int i;
	int j;
	for (i = 0; i < Lista_Particulas[0].c_Particulas; i++)
	{
		for (j = 0; j < cantidad_celdas; j++)
		{
			Array_Celdas[j] = FormulaYComparacion(cantidad_celdas, Array_Celdas[j], j, Lista_Particulas[i].energia, Lista_Particulas[i].posicion);
		}
	}

	printf("Despues del impacto de las particulas en las celdas\n");
	MostrarCeldas(Array_Celdas, cantidad_celdas);
	return 0;
}
