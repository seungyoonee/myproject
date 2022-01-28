CC  = gcc
CXX = g++

INCLUDES = 
CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g
LDLIBS = 

.PHONY: default
default: http-client

# header dependency
http-client.o: 

.PHONY: clean
clean:
	rm -f *.o http-client

.PHONY: all
all: clean default

.PHONY: gnu
gnu: http-client
	./http-client www.gnu.org 80 /software/make/manual/make.html

.PHONY: vg
vg: http-client
	valgrind --leak-check=yes ./http-client www.gnu.org 80 /software/make/manual/make.html
