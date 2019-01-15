CC = gcc
CFLAGS = -pedantic -ansi -std=c11 -Wall -Iinclude/
COMMITM = new Commit

all: parser main

git: gitAdd gitCommit gitPush

pull: gitStash gitPull

run: runMain

parser:
	$(CC) $(CFLAGS) -c ./src/CalendarParser.c -o ./bin/parser.o

main:
	$(CC) $(CFLAGS) ./bin/parser.o -o ./bin/main.o ./src/main.c


runMain:
	./bin/main.o

runMainVal:
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