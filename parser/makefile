CC = gcc
CFLAGS =  -Wall -g -std=c11 -Iinclude/
COMMITM = new Commit
SRC = src/
BIN = bin/

all: list parser

list: ./src/LinkedListAPI.c
	$(CC) $(CFLAGS) -fpic -c ./src/LinkedListAPI.c -o ./bin/list.o
	$(CC) $(CFLAGS) -dynamiclib ./bin/list.o -o ./bin/liblist.dylib

parser: ./src/helper.c ./src/CalendarParser.c
	$(CC) $(CFLAGS) -fpic -c ./src/helper.c -o ./bin/helper.o
	$(CC) $(CFLAGS) -fpic -c ./src/CalendarParser.c  -o ./bin/parser.o
	$(CC) $(CFLAGS) -fpic -c ./src/LinkedListAPI.c -o ./bin/list.o
	$(CC) $(CFLAGS) -dynamiclib ./bin/list.o ./bin/helper.o ./bin/parser.o -o ../libcal.dylib

clean:
	rm -rf ./bin/*.o
	rm -rf ./bin/*.so
