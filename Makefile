CC := g++
CFLAGS := -g -std=c++11

all:
	$(CC) src/main.cpp -o bin/main $(CFLAGS)