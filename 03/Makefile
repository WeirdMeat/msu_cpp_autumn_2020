CC=g++
CFLAGS=-c

.PHONY: all test

all: main

test: tested
		$^

main: matrix.o main.o
		$(CC) $^ -o $@

tested: tested.o matrix.o
		$(CC) $^ -o $@

%.o: %.cpp
		$(CC) $(CFLAGS) $< -o $@
