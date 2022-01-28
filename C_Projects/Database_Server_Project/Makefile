CC = gcc
CXX = g++

INCLUDES =
CFLAGS = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g
LDLIPBS =

.PHONY: default
default: mdb-lookup-server-nc-2 mdb-lookup-server-nc-1

.PHONY: clean
clean:
	rm -f *.o mdb-lookup-server-nc-2 mdb-lookup-server-nc-1

.PHONY: all
all: clean default

