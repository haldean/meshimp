BINARY=meshimp
CFLAGS=-c -Wall -g -I../include/ --std=c++0x -I/usr/include/GL -I/usr/include -O0
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
UNAME=$(shell uname)

ifeq ($(UNAME), Darwin)
CC=clang++
LDFLAGS=-lm -framework OpenGL -framework GLUT -pg
else
CC=g++
LDFLAGS=-L/usr/local/lib -L/usr/X11/lib -pg
endif

all: $(SOURCES) $(BINARY)
	
$(BINARY): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ${OBJECTS} ${BINARY}
