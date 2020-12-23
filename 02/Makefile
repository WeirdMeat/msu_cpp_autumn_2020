CC=g++ -std=c++17
CFLAGS=-c

.PHONY: all test

all: main

test: tested
		@echo testing...
		@./$<
		@echo testing successful

main: parser.o main.o
		$(CC) $^ -o $@

tested: tested.o parser.o
		$(CC) $^ -o $@

%.o: %.cpp
		$(CC) $(CFLAGS) $< -o $@
