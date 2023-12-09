FLAGS = -Wall -pthread -lm

main: lab3.c funciones.o
	gcc $(FLAGS) -o lab3 lab3.c funciones.o

funciones.o: funciones.c
	gcc -c $(FLAGS) funciones.c

clear:
	rm -f lab3 *.o