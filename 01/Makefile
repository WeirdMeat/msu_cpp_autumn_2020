CC=g++
CFLAGS=-c

all: main

test: test

main: allocator.o main.o
		$(CC) $^ -o $@

test: test.o allocator.o
		$(CC) $^ -o $@

%.o: %.cpp
		$(CC) $(CFLAGS) $< -o $@
