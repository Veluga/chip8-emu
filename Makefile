CC := g++
CFLAGS := -g -std=c++17
INCLUDE := include
SRCDIR := src
TARGET := bin/main

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

all: $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) -I $(INCLUDE) -o $(TARGET)

clean:
	rm -rf bin/*

.PHONY: all clean