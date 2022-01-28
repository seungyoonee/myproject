
#
# Makefile for lab 6, part 1
#

CC  = gcc
CXX = g++

INCLUDES = -I../../lab3/solutions/part1
CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g -L../../lab3/solutions/part1
LDLIBS = -lmylist

.PHONY: default
default: mdb-lookup-server 

mdb-lookup-server: mdb.o

# header dependency
mdb-lookup-server.o: mdb.h
mdb.o: mdb.h 

.PHONY: clean
clean:
	rm -f *.o *~ a.out core mdb-lookup-server

.PHONY: all
all: clean default

