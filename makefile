CC = gcc
CFLAGS = -Wall -g

all: dispatcher

dispatcher: dispatcher.c queue.c
	$(CC) $(CFLAGS) -o dispatcher dispatcher.c queue.c

clean:
	rm -f dispatcher

