CC=g++ -std=c++17
CFLAGS=-c

.PHONY: all test

all: main

test: tested
		@echo testing...
		@./$?
		@echo testing successful

main: allocator.o main.o
		$(CC) $? -o $@

tested: tested.o allocator.o
		$(CC) $? -o $@

.cpp.o:
		$(CC) $(CFLAGS) $? -o $@
