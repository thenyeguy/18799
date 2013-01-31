CC     = gcc
CFLAGS = -c -Wall -std=c99
OFLAGS = -lportaudio

all: test

test: test.o
	$(CC) $(OFLAGS) -o test test.o

test.o: test.c
	$(CC) $(CFLAGS) test.c

clean:
	rm -rf *.o test
