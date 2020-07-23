all: compile

compile: dungeon.cpp generator.cpp load_save.cpp render.cpp path_finding.cpp heap.cpp
	g++ -Wall -Werror -ggdb2 -O0 dungeon.cpp generator.cpp load_save.cpp render.cpp path_finding.cpp npc.cpp heap.cpp rlg327.cpp -o bin/dungeon -lm -lncurses

run: bin/dungeon
	bin/dungeon

clean:
	rm -rf bin/* *.pgm
