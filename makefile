CC = gcc
CFLAGS =  -Wall -g -std=c11 -Iinclude/
COMMITM = new Commit
SRC = src/
BIN = bin/

all: list helper parser main

git: gitAdd gitCommit gitPush

pull: gitStash gitPull

list:
	$(CC) $(CFLAGS) -c ./src/LinkedListAPI.c -o ./bin/LL.o

helper:
	$(CC) $(CFLAGS) -c ./src/helper.c -o ./bin/helper.o

parser:
	$(CC) $(CFLAGS) -c ./src/CalendarParser.c -o ./bin/parser.o

main:
	$(CC) $(CFLAGS) ./bin/parser.o ./bin/LL.o -o ./bin/main.o ./src/main.c

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