CC=g++
CFLAGS=-std=c++11
LDFLAGS=-L/usr/lib/x86_64-linux-gnu -lgmp

all: program

program: main.cpp
	$(CC) $(CFLAGS) -o program main.cpp $(LDFLAGS)

clean:
	rm -f program
