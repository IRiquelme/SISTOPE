#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t readLock;
pthread_mutex_t writeLock;

// struct para argumentos de hebras
typedef struct
{
    FILE *archivo;
    int chunk;
    int flagPrimeraLinea;
    double *arregloCeldas;
} ArgumentosHebra;

int main(int argc, char *argv[])
{
    // Option para el getopt
    int option;

    // Variables int
    int N = 0, D = 0, H = 0, c = 0;

    // Variable string
    char i[100];
    char o[100];

    // Variables booleanas, si se mantienen en 0, es porque no fueron ingresadas por lo tanto pasan por un IF, donde verifican aquello
    int obligatorio_N, obligatorio_o, obligatorio_i, obligatorio_H, obligatorio_c = 0;

    while ((option = getopt(argc, argv, "N:H:i:o:c:D")) != -1)
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

            obligatorio_N = 1;
            break;

        case 'H':
            H = atoi(optarg);
            // Valida si H es un numero, y si es al menos  uno. Debe existir al menos una hebra para procesar el archivo
            if (P < 1)
            {
                printf("Porfavor ingrese un valor entero positivo y valido de workers, el cual corresponde al parametro -H");
                exit(0);
            }
            obligatorio_H = 1;
            break;

        case 'c':
            c = atoi(optarg);
            // Valida si c es un numero, y si es al menos uno. La cantidad minima de lineas a leer debe ser uno
            if (c < 1)
            {
                printf("Porfavor ingrese un valor entero positivo y valido de chunks, el cual corresponde al parametro -c");
                exit(0);
            }
            obligatorio_c = 1;
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
    if (obligatorio_H == 0)
    {
        printf("Se debe ingresar el valor del parametro -H\n");
        exit(0);
    }
    if (obligatorio_c == 0)
    {
        printf("Se debe ingresar el valor del parametro -c\n");
        exit(0);
    }



    pthread_t* arregloHebras = (pthread_t*)malloc(sizeof(pthread_t)*H);

    ArgumentosHebra args;
    args.archivo = fopen(i, "r");
    args.chunk = c;
    args.flagPrimeraLinea = 1;
    args.arregloCeldas = (double *)calloc(N, sizeof(double));       // Con calloc nos aseguramos de que este inicializado en 0 cada valor del arreglo

    for(int i = 0; i < H; i++){
        pthread_create(&arregloHebras[i], NULL, procesar_archivo, (void*)(&args));
    }

    for(int i = 0; i < H; i++){
        pthread_join(arregloHebras[i], NULL);
    }

    fclose(args.archivo);
    free(args.arregloCeldas);
    free(args.lineasTrabajasPorHebra);
    return 0;
}

void* procesar_archivo(void* args){
    ArgumentosHebra* argumentos = (ArgumentosHebra*)args;
    int i,j;
    int lineasEnBuffer;
    int lineasTrabajadas = 0;
    int finalArchivo = 0;

    char** buffer = (char**)calloc(argumentos->chunk, sizeof(char*)); // Esta matriz representa un chunk de lineas del archivo

    for(i = 0; i < argumentos->chunk; i++){
        buffer[i] = (char*)calloc(50, sizeof(char)); // Con calloc nos aseguramos de que este inicializado en 0 cada valor del arreglo
    }

    while(1){
        lineasEnBuffer = 0;

        pthread_mutex_lock(&readLock); // EnterCS()

        if(argumentos->flagPrimeraLinea == 1){
                argumentos->flagPrimeraLinea = 0;
                fgets(buffer[0], sizeof(buffer[0]), argumentos->archivo); // Descartamos la primera linea del archivo, ya que no se utilizara
        }

        for(i = 0; i < argumentos->chunk; i++){
            
            if(fgets(buffer[i], sizeof(buffer[i]), argumentos->archivo) == NULL){
                finalArchivo = 1;
                break;
            }
            lineasEnBuffer++;
        }

        pthread_mutex_unlock(&readLock); // ExitCS()

        if (lineasBuffer > 0)
        {
            pthread_mutex_lock(&writeLock); // EnterCS()

            int posicionParticula;
            double energiaParticula;
            for (i = 0; i < lineasEnBuffer; i++)
            {
                sscanf(buffer[i], "%d %lf", &posicionParticula, &energiaParticula);
                for (j = 0; j < argumentos->chunk; j++)
                {
                    argumentos->arregloCeldas[j] = formula(argumentos->chunk, argumentos->arregloCeldas[j], j, energiaParticula, posicionParticula); // revisar
                }
                lineasTrabajadas++;
            }

            pthread_mutex_unlock(&writeLock); // ExitCS()
        }

        if(finalArchivo == 1){
            break;
        }
    }

    return lineasTrabajadas;
} 