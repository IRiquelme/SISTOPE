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
            // Verificacion del archivo de entrada
            FILE *fp = fopen(i, "r");
            if (fp == NULL) // Verificamos que el archivo exista
            {
                printf("El archivo indicado en -i no existe.\n");
                exit(0);
            }

            if (fgetc(fp) == EOF) // Verificamos que el archivo no este vacio
            {
                printf("El archivo indicado en -i está vacío.\n");
                exit(0);
            }
            fclose(fp);

            obligatorio_i = 1;
            break;
        case 'o':
            strcpy(o, optarg);
            // Falta completar?
            obligatorio_o = 1;
            break;
        case 'D':
            // Si se encuentra la D; quiere decir que se quiere mostrar por consola el resultado, mas abajo esta el if de eso
            D = 1;
            break;
        }
    }
    // Verificamos si se ingresaron las flags obligatorias
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

    // Desde el archivo especificado en la flag -i obtenemos las cantidad de particulas y una lista de particulas
    int cantidad_Particulas;
    Particula *Lista_Particulas = lecturaParticulas(i, &cantidad_Particulas);

    // Array que representa el material con el cual colisionaran las particulas
    double *Array_Celdas = (double *)calloc(N, sizeof(double)); // Inicializar el array con solo 0

    int k;
    int j;
    for (k = 0; k < cantidad_Particulas; k++)
    {
        for (j = 0; j < N; j++)
        {
            // Se actualiza la energia en las celdas del material por cada particula de la lista obtenida con anterioridad.
            Array_Celdas[j] = formulaYComparacion(N, Array_Celdas[j], j, Lista_Particulas[k].energia, Lista_Particulas[k].posicion);
        }
    }
    // Registramos los resultados obtenidos en el archivo de salida especificado.
    escrituraArchivo(Array_Celdas, N, o);

    if (D)
    {
        // En caso de que se agregue la flag -D, se mostrara por consola la energia final en cada celda del material
        mostrarCeldas(Array_Celdas, N);
    }

    return 0;
}