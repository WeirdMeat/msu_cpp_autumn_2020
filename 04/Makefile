CC=g++ -std=c++17
CFLAGS=-c

.PHONY: all test clean

all: main

test: tested
		@echo testing...
		@./$?
		@echo testing successful

main: big_numbers.o main.o
		$(CC) $? -o $@

tested: tested.o big_numbers.o
		$(CC) $? -o $@

.cpp.o:
		$(CC) $(CFLAGS) $?

clean:
		rm -rf main tested *.o
