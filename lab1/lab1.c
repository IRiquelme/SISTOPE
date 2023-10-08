#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//===============================================================
//					   Struct de particulas
//===============================================================
struct Particulas
{
	int c_Particulas;
	int posicion;
	float energia;
};
typedef struct Particulas Particulas;

//===============================================================
//					   Funciones de lectura y formula
//===============================================================
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
		printf("Celda: %d,Energia: %0.6f \n", i, array_Celdas[i]);
	}
}

int main()
{
	char nombreInput[20] = "test1_35.txt"; // Nombre o variable donde el archivo de entrada de particulas se llama
	Particulas *Lista_Particulas = Lectura_Particulas(nombreInput);

	int cantidad_celdas = 35; // Nombre o variable donde se estarian almacenando las celdas que se establecen del 1 al 6

	float *Array_Celdas = (float *)calloc(cantidad_celdas, sizeof(float)); // Inicializar el array con solo 0

	printf("Antes de el impacto de las particulas\n");
	MostrarCeldas(Array_Celdas, cantidad_celdas);

	// LA MAGIA, en los siguientes 2 for, se implementa el calculo de cuanto impacta en energia cada particula en las celdas

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