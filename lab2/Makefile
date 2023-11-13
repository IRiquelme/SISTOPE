FLAGS = -Wall -g

all: lab2 broker worker

lab2: lab2.c
	$(CC) $(FLAGS) -o lab2 lab2.c

broker: broker.c fbroker.o
	$(CC) $(FLAGS) -o broker broker.c fbroker.o

fbroker.o: fbroker.c
	$(CC) -c $(FLAGS) fbroker.c

worker: worker.c fworker.o
	$(CC) $(FLAGS) -o worker worker.c fworker.o -lm

fworker.o: fworker.c
	$(CC) -c $(FLAGS) fworker.c

clear:
	rm -f lab2 broker worker *.o