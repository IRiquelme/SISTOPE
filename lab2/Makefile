CC = gcc
CFLAGS = -Wall -g

all: lab2 broker worker

lab2: lab2.c
	$(CC) $(CFLAGS) -o lab2 lab2.c

broker: broker.c
	$(CC) $(CFLAGS) -o broker broker.c

worker: worker.c
	$(CC) $(CFLAGS) -o worker worker.c -lm

clean:
	rm -f lab2 broker worker