#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "particula.h"

int main(int argc, char const *argv[])
{
    char buffer[25];
    int lineasRecibidas;
    while (1)
    {
        if (read(STDIN_FILENO, buffer, size_of(buffer)) > 0)
        {
            lineasRecibidas++;
        }
    }

    int pid = getpid();

    char mensaje[100];
    sprintf(mensaje, "Soy el worker %d y he recibido %d líneas del broker", pid, lineasRecibidas);
    write(STDOUT_FILENO, mensaje, size_of(mensaje));

    return 0;
}