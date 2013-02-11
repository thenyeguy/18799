CC = gcc
CFLAGS = -c -Wall -Wextra -g #-Werror
OFLAGS = -o
SOURCES = spell_checker_main.c trellis_utils.c levenshtein_utils.c
OBJECTS = $(SOURCES:.c=.o)

#all: spell_checker_main.c
#	gcc spell_checker_main.c -o spell_checker

spell_checker: ${OBJECTS}
	${CC} ${OBJECTS} ${OFLAGS} spell_checker 

spell_checker_main.o: spell_checker_main.c spell_checker_main.h trellis_utils.c trellis_utils.h
	${CC} ${CFLAGS} spell_checker_main.c

trellis_utils.o: trellis_utils.h trellis_utils.c
	${CC} ${CFLAGS} trellis_utils.c 

levenshtein_utils.o: levenshtein_utils.h levenshtein_utils.c
	${CC} ${CFLAGS} levenshtein_utils.c

clean:
	rm -f *.o
 
