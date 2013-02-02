CC      = gcc
OFLAGS  = -c -Wall -Werror -std=c99
CFLAGS  = -lportaudio
SOURCES = portaudio_utils.c endpoint.c test.c
OBJECTS = $(SOURCES:.c=.o)

all: test

test: $(OBJECTS)
	$(CC) $(CFLAGS) -o test.out $(OBJECTS)

%.o: %.c
	$(CC) $(OFLAGS) $<

clean:
	rm -rf *.o *.out
