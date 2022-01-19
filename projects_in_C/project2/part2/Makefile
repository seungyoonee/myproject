CC = gcc
CFLAGS = -Wall -g
LDLIBS =

twecho: twecho.o

twecho.o:

.PHONY: clean
clean:
	rm -f *.o twecho

.PHONY: all
all: clean twecho

.PHONY: val
val: twecho
	valgrind --leak-check=yes ./twecho hello world dude

