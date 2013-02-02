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

#test.o: test.c
#	$(CC) $(OFLAGS) test.c

#portaudio_utils.o: portaudio_utils.c
#	$(CC) $(OFLAGS) portaudio_utils.c

clean:
	rm -rf *.o *.out
