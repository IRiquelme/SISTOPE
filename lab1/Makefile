FLAGS = -Wall -lm

main:   lab1.c funciones.o
    gcc $(FLAGS) -o lab1 lab1.c funciones.o

funciones.o:    funciones.c particula.h
    gcc -c $(FLAGS) funciones.c

lab1.o: lab1.c particula.h
    gcc -c $(FLAGS) lab1.c

clear:
    rm -f lab1 *.o