#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    // Option para el getopt
    int option;

    // Variables int
    int N = 0, D = 0, P = 0, c = 0;

    // Variable string
    char i[100];
    char o[100];

    // Variables booleanas, si se mantienen en 0, es porque no fueron ingresadas por lo tanto pasan por un IF, donde verifican aquello
    int obligatorio_N, obligatorio_o, obligatorio_i, obligatorio_P, obligatorio_c = 0;

    while ((option = getopt(argc, argv, "N:P:c:i:o:D")) != -1)
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

        case 'P':
            P = atoi(optarg);
            // Valida si P es un numero, y si es al menos  uno. Debe existir al menos un worker que lea el archivo
            if (P < 1)
            {
                printf("Porfavor ingrese un valor entero positivo y valido de workers, el cual corresponde al parametro -P");
                exit(0);
            }
            obligatorio_P = 1;
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
    if (obligatorio_P == 0)
    {
        printf("Se debe ingresar el valor del parametro -P\n");
        exit(0);
    }
    if (obligatorio_c == 0)
    {
        printf("Se debe ingresar el valor del parametro -c\n");
        exit(0);
    }

    // CREAR BROKER
    // Se crea el broker, el cual se encargara de leer el archivo y enviar las particulas a los workers
    int pid_broker = fork();
    if (pid_broker == 0)
    {
        char N_str[100];
        char D_str[100];
        char P_str[100];
        char c_srt[100];
        sprintf(N_str, "%d", N);
        sprintf(D_str, "%d", D);
        sprintf(P_str, "%d", P);
        sprintf(c_srt, "%d", c);
        char *args[] = {"./broker", N_str, P_str, D_str, c_srt, i, o, NULL};
        execv(args[0], args);
        exit(0);
    }
    wait(NULL);

    return 0;
}