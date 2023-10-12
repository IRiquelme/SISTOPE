FLAGS = -Wall

main: lab1.c funciones.o
	gcc $(FLAGS) -o lab1 lab1.c funciones.o -lm

funciones.o: funciones.c
	gcc -c $(FLAGS) funciones.c

clear:
	rm -f lab1 *.o lab1
