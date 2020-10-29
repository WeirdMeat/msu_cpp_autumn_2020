CC=g++ -std=c++11
CFLAGS=-c

.PHONY: all test clean

all: main

test: tested
		./$<

main: matrix.o main.o
		$(CC) $^ -o $@

tested: tested.o matrix.o
		$(CC) $^ -o $@

%.o: %.cpp
		$(CC) $(CFLAGS) $<

clean:
		rm -rf main tested *.o
