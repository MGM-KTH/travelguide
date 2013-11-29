CC=gcc 
CFLAGS=-Wall
DEBUG=-g

all: main

debug: CC += -DDEBUG -g
debug: all

main: main.o
	$(CC) $(CFLAGS) main.o -o main -lm
	rm *.o

clean:
	rm -f main
