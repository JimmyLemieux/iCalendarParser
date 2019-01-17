CC = gcc
CFLAGS = -pedantic -ansi -std=c11 -Wall -Iinclude/
COMMITM = new Commit

all: helper parser main

git: gitAdd gitCommit gitPush

pull: gitStash gitPull

helper:
	$(CC) $(CFLAGS) -c ./src/helper.c -g -o ./bin/helper.o

parser:
	$(CC) $(CFLAGS) -c ./src/CalendarParser.c -g -o ./bin/parser.o

main:
	$(CC) $(CFLAGS) ./bin/parser.o -g -o ./bin/main.o ./src/main.c


runM:
	./bin/main.o

runMVal:
	valgrind ./bin/main.o

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