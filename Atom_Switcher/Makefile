CC = g++
VERSION = -std=c++11
CFLAGS = $(VERSION) -c -Wall

all:	hello

hello:	Source.o
	$(CC) Source.o -o hello

Source.o:	Atom.h Source.cpp
		$(CC) $(CFLAGS) Atom.h Source.cpp

clean:
	rm -rf *o hello