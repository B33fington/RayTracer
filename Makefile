# Compiler:
CC = g++
CFLAGS = -Wall

# Files:
FILES = main.cpp
#or
# FILES = $(wildcard *.cpp)

BINNAME = main.exe

all: compile
.PHONY: all

compile: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $(BINNAME)
.PHONY: compile

run:
	$(BINNAME)
.PHONY: run

clean:
	rm -f $(BINFOLD)*
.PHONY: clean