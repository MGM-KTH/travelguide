CC=gcc 
CFLAGS=-Wall

all: main

main: main.o
	$(CC) $(CFLAGS) main.o -o main -lm
	rm *.o

clean:
	rm -f main
