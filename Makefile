CC      = gcc
OFLAGS  = -c -Wall -Werror -std=c99 -g 
CFLAGS  = -lportaudio
SOURCES = cepstrum_vectors.c cepstrum_utils.c wav_utils.c \
          portaudio_utils.c endpoint.c
OBJECTS = $(SOURCES:.c=.o)

all: record cepstrum
full: clean all

record: $(OBJECTS) record.o
	$(CC) $(CFLAGS) -o record.out $(OBJECTS) record.o

record2: $(OBJECTS) record2.o
	$(CC) $(CFLAGS) -o record2.out $(OBJECTS) record2.o

cepstrum: $(OBJECTS) cepstrum.o
	$(CC) $(CFLAGS) -o cepstrum.out $(OBJECTS) cepstrum.o

%.o: %.c
	$(CC) $(OFLAGS) -o $@ $<

clean:
	rm -rf *.o *.out
