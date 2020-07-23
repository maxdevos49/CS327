all: c_debug

c_debug: dungeon_generator.c headers/dungeon_generator.h
	gcc -Wall -Werror -ggdb2 -O0 dungeon_generator.c -o bin/debug/dungeon -lm

c_release: dungeon_generator.c headers/dungeon_generator.h
	gcc -O2 dungeon_generator.c -o bin/release/dungeon -lm

run: bin/debug/dungeon
	bin/debug/dungeon

release: c_release bin/release/dungeon
	bin/release/dungeon
	
clean:
	rm -rf bin/release/* bin/debug/*