CC=g++
CFLAGS=-c

.PHONY: all test

all: main

test: tested
		$^

main: allocator.o main.o
		$(CC) $^ -o $@

tested: tested.o allocator.o
		$(CC) $^ -o $@

%.o: %.cpp
		$(CC) $(CFLAGS) $< -o $@
