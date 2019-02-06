CC = gcc
CFLAGS =  -Wall -g -std=c11 -Iinclude/
COMMITM = new Commit
SRC = src/
BIN = bin/

all: list parser

git: gitAdd gitCommit gitPush

pull: gitStash gitPull

list: ./src/LinkedListAPI.c ./include/LinkedListAPI.h
	$(CC) $(CFLAGS) -fpic -c ./src/LinkedListAPI.c -o ./bin/list.o
	$(CC) $(CFLAGS) -shared ./bin/list.o -o ./bin/liblist.so

parser: ./src/helper.c ./src/CalendarParser.c ./include/helper.h ./include/CalendarParser.h
	$(CC) $(CFLAGS) -fpic -c ./src/helper.c -o ./bin/helper.o
	$(CC) $(CFLAGS) -fpic -c ./src/CalendarParser.c  ./bin/helper.o -o ./bin/parser.o
	$(CC) $(CFLAGS) -shared ./bin/parser.o -o ./bin/libparser.so

clean:
	rm -rf ./bin/*.o

gitAdd:
	git add .

gitCommit:
	git commit -m "$m"

gitPush:
	git push -u origin master

gitStash:
	git stash save --keep-index

gitPull:
	git pull https://JimmyLemieux:Jrrangers123321@github.com/JimmyLemieux/iCalendarParser.git