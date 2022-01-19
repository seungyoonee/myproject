CC = gcc
CFLAGS = -Wall -g
LDLIBS = -lm

main: main.o gcd.o prime.o

main.o: gcd.h prime.h

gcd.o: gcd.h

prime.o: prime.h

.PHONY: clean
clean:
	rm -f *.o main

.PHONY: all
all: clean main

