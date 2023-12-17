#ifndef argumentosHebra_H
#define argumentosHebra_H

typedef struct
{
    int idHebra;
    FILE *archivo;
    double *arregloCeldas;
    int *lineasTrabajadas;
    int* flags;
    int chunk;
    int largoCeldas;
    pthread_mutex_t *semaforos;
} ArgumentosHebra;

#endif