CC=g++
CFLAGS=-c

.PHONY: all test

all: main

test: tested
		$^

main: parser.o main.o
		$(CC) $^ -o $@

tested: tested.o parser.o
		$(CC) $^ -o $@

%.o: %.cpp
		$(CC) $(CFLAGS) $< -o $@
