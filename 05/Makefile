CC=g++ -std=c++17
CFLAGS=-c

.PHONY: all test clean

all: main

test: tested
		@echo testing...
		@./$<
		@echo testing successful

main: serializer.o main.o
		$(CC) $^ -o $@

tested: tested.o serializer.o
		$(CC) $^ -o $@

%.o: %.cpp
		$(CC) $(CFLAGS) $<

clean:
		rm -rf main tested *.o
