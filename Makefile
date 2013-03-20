# Recursively looks for matches
percent_subdirs = 1

# Specify compiler and output locations
CC      = gcc
SRCDIR  = src
OBJDIR  = obj
BINDIR  = bin

# Automagically crawl the directory structure and find our source files
SRCDIRS = $(shell ls src)
SOURCES = $(shell find ${SRCDIR} \( -name "*.c" -and -not -name "*main.c" \) )
OBJECTS = $(patsubst %.c,%.o,${SOURCES})

# Automagically crawl the directory structure and find the compilable mains!
OBJLOCS = $(addprefix ${OBJDIR}/,$(shell basename ${OBJECTS}))
MAINSOURCES = $(shell find ${SRCDIR} \( -name "*.c" -and -name "*main.c" \))
MAINS   = $(patsubst %_main.c,%,$(shell basename $(MAINSOURCES)))

# Compiler flags
CFLAGS  = -Wall -Werror -std=c99 -g $(addprefix -I./${SRCDIR}/,${SRCDIRS})
LFLAGS  = -lportaudio -L./${OBJDIR}/


# Handy shortcuts for make
all: $(MAINS)
full: clean all


# Autogenerate compilation of main files
$(MAINS): ${BINDIR} $(OBJECTS)
	@echo Building bin/$@
	@${CC} ${CFLAGS} -c $(shell find ${SRCDIR} \( -name "$@_main.c" \) ) \
		-o ${OBJDIR}/$@_main.o
	@$(CC) $(LFLAGS) ${OBJLOCS} ${OBJDIR}/$@_main.o -o ${BINDIR}/$@ 


# Autogenerate compilation of object files
%.o: %.c ${OBJDIR}
	@echo Compiling $(@F)...
	@${CC} ${CFLAGS} -c $< -o ${OBJDIR}/$(@F)


# Manage generation and cleaning of output directories
${OBJDIR}:
	@mkdir -p ${OBJDIR}
${BINDIR}:
	@mkdir -p ${BINDIR}
clean:
	@rm -rf ${OBJDIR} 
	@rm -rf ${BINDIR}
