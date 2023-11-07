#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    int N = atoi(argv[1]);  // N es el numero de celdas
    int P = atoi(argv[2]);  // P es el numero de workers
    int D = atoi(argv[3]);  // D indica si se imprime por consola el resultado
    char i[100] = argv[4];  // i es el nombre del archivo de entrada
    char o[100] = argv[5];  // o es el nombre del archivo de salida

    printf("SOY EL BROKER\n");
    printf("N: %d\n", N);
    printf("P: %d\n", P);
    printf("D: %d\n", D);
    printf("i: %s\n", i);
    printf("o: %s\n", o);
    return 0;
}