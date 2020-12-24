CC=g++ -std=c++17 -pthread
CFLAGS=-c

.PHONY: all test clean

all: main

test: tested
		@echo testing...
		@./$<
		@echo testing successful

main: thread_pool.o main.o
		$(CC) $^ -o $@

tested: thread_pool.o tested.o
		$(CC) tested.o -o $@

%.o: %.cpp
		$(CC) $(CFLAGS) $<

clean:
		rm -rf main tested *.o
