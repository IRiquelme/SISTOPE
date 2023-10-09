#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "funciones.h"
#include "particula.h"

int main(int argc, char *argv[])
{

    // Option para el getopt
    int option;

    // Variables int
    int N = 0, D = 0;

    // Variable string
    char i[100];
    char o[100];

    // Variables booleanas, si se mantienen en 0, es porque no fueron ingresadas por lo tanto pasan por un IF, donde verifican aquello
    int obligatorio_N, obligatorio_o, obligatorio_i = 0;

    while ((option = getopt(argc, argv, "N:i:o:D")) != -1)
    {
        switch (option)
        {
        case 'N':
            N = atoi(optarg);
            // Valida si N es un numero, y si es un entero positivo
            if (N <= 0)
            {
                printf("Porfavor ingrese un valor entero positivo y valido de celdas, el cual corresponde al parametro -N");
                exit(0);
            }
            // Si fue encontrada o ingresada
            obligatorio_N = 1;
            break;
        case 'i':
            strcpy(i, optarg);
            // Falta completar

            obligatorio_i = 1;
            break;
        case 'o':
            strcpy(o, optarg);
            // Falta completar

            obligatorio_o = 1;
            break;
        case 'D':
            // Si se encuentra la D; quiere decir que se quiere mostrar por consola el resultado, mas abajo esta el if de eso
            D = 1;
            break;
        }
    }
    if (obligatorio_N == 0)
    {
        printf("Se debe ingresar el valor del parametro -N\n");
        exit(0);
    }
    if (obligatorio_i == 0)
    {
        printf("Se debe ingresar el valor del parametro -i\n");
        exit(0);
    }
    if (obligatorio_o == 0)
    {
        printf("Se debe ingresar el valor del parametro -o\n");
        exit(0);
    }

    if (fopen(i, "r") == NULL)
    {
        printf("El archivo indicado en -i no existe.\n");
        exit(0);
    }

    if (fgetc(fopen("archivo.txt", "r")) == EOF)
    {
        printf("El archivo indicado en -i está vacío.\n");
    }

    Particulas *Lista_Particulas = Lectura_Particulas(i);

    double *Array_Celdas = (double *)calloc(N, sizeof(double)); // Inicializar el array con solo 0

    int k;
    int j;
    for (k = 0; k < Lista_Particulas[0].c_Particulas; k++)
    {
        for (j = 0; j < N; j++)
        {
            Array_Celdas[j] = FormulaYComparacion(N, Array_Celdas[j], j, Lista_Particulas[k].energia, Lista_Particulas[k].posicion);
        }
    }
    EscrituraArchivo(Array_Celdas, N, o);

    if (D)
    {
        MostrarCeldas(Array_Celdas, N);
    }

    return 0;
}
