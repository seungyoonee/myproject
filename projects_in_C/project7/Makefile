CC  = gcc
CXX = g++

INCLUDES =
CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g
LDLIBS =

.PHONY: default
default: http-server

# header dependency
http-server.o: 

.PHONY: clean
clean:
	rm -f *.o http-server

.PHONY: all
all: clean default

.PHONY: test1
test1: default
	./http-server 21647 ~/html/cs3157 127.0.0.1 20647

.PHONY: test2
test2: default
	./http-server 31647 ~/html/cs3157 127.0.0.1 20647

.PHONY: vg1
vg1: default
	valgrind --leak-check=yes ./http-server 21647 ~/html/cs3157 127.0.0.1 20647

.PHONY: vg2
vg2: default
	valgrind --leak-check=yes ./http-server 31647 ~/html/cs3157 127.0.0.1 20647
