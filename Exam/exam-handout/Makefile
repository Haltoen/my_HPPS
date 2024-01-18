CC?=cc
CFLAGS?=-Wall -Wextra -pedantic -lm -fopenmp -O3 -g 
PROGRAMS= genparticles particles2text nbody nbody-bh

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS) *.o

%: %.o util.o
	$(CC) -o $@ $*.o util.o $(CFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)


.SUFFIXES: # Disables implicit rules.  (Don't worry about it.)
