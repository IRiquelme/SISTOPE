FLAGS = -Wall -g

all: broker worker lab2

lab2: lab2.c
	gcc $(FLAGS) -o lab2 lab2.c

broker: broker.c fbroker.o
	gcc $(FLAGS) -o broker broker.c fbroker.o

fbroker.o: fbroker.c
	gcc -c $(FLAGS) fbroker.c

worker: worker.c fworker.o
	gcc $(FLAGS) -o worker worker.c fworker.o -lm

fworker.o: fworker.c
	gcc -c $(FLAGS) fworker.c

clear:
	rm -f lab2 broker worker *.o
