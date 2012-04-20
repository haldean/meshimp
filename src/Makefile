BINARY=meshimp
CC=clang++
CFLAGS=-c -Wall -g -I../include/ -O3
LDFLAGS=-lpng -L/usr/local/lib -L/usr/X11/lib
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(BINARY)
	
$(BINARY): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ${OBJECTS} ${BINARY}
