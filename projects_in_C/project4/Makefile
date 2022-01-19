CC = gcc
CXX = g++

INCLUDES = -I../../lab3/solutions/part1
CFLAGS	 = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g -L../../lab3/solutions/part1
LDLIBS = -lmylist

mdb-lookup:

mdb-lookup.o: mdb.h

.PHONY: clean
clean:
	rm -f *.o mdb-lookup

.PHONY: all
all: clean mdb-lookup

