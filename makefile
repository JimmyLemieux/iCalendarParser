CC = gcc
CFLAGS = -pedantic -ansi -std=c11 -Wall -Iinclude/

all: parser main

parser:
	$(CC) $(CFLAGS) -c ./src/CalendarParser.c -o ./bin/parser.o

main:
	$(CC) $(CFLAGS) ./bin/parser.o -o ./bin/main.o ./src/main.c
