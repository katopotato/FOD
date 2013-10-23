all:
	gcc -c hunter.c HunterView.c Graph.c -ljansson

clean:
	rm -rf myPlayer
