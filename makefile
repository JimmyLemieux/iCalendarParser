git: gitAdd gitCommit gitPush

pull: gitStash gitPull

gitAdd:
	git add .

gitCommit:
	git commit -m "some push"

gitPush:
	git push -u origin master

gitStash:
	git stash save --keep-index

gitPull:
	git pull https://JimmyLemieux:Jrrangers123321@github.com/JimmyLemieux/iCalendarParser.git
