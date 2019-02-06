CC = gcc
CFLAGS = -std=c11 -Wall -Iinclude/
COMMITM = new Commit

all: LL helper parser main

git: gitAdd gitCommit gitPush

pull: gitStash gitPull

LL:
	$(CC) $(CFLAGS) -c ./src/LinkedListAPI.c -g -o ./bin/LL.o

helper:
	$(CC) $(CFLAGS) -c ./src/helper.c -g -o ./bin/helper.o

parser:
	$(CC) $(CFLAGS) -c ./src/CalendarParser.c -g -o ./bin/parser.o

main:
	$(CC) $(CFLAGS) ./bin/parser.o ./bin/LL.o -g -o ./bin/main.o ./src/main.c


clean:
	rm -rf ./bin/*.o

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