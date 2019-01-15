CC = gcc
CFLAGS = -pedantic -ansi -std=c11 -Wall -Iinclude/
COMMITM = new Commit

all: parser main

git: gitAdd gitCommit gitPush

parser:
	$(CC) $(CFLAGS) -c ./src/CalendarParser.c -o ./bin/parser.o

main:
	$(CC) $(CFLAGS) ./bin/parser.o -o ./bin/main.o ./src/main.c


gitAdd:
	git add .

gitCommit:
	git commit -m "$m"

gitPush:
	git push -u origin master
