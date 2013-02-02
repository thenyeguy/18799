CC     = gcc
OFLAGS = -c -Wall -Werror -std=c99 -g
CFLAGS = -lportaudio
SOURCES = portaudio_utils.c test.c wav_utils.c
OBJECTS = $(SOURCES:.c=.o)

all: test

test: test.o portaudio_utils.o wav_utils.o
	$(CC) $(CFLAGS) -o test.out $(OBJECTS)

test.o: test.c
	$(CC) $(OFLAGS) test.c

portaudio_utils.o: portaudio_utils.c
	$(CC) $(OFLAGS) portaudio_utils.c

wav_utils.o: wav_utils.c
	$(CC) $(OFLAGS) wav_utils.c

clean:
	rm -rf *.o *.out
