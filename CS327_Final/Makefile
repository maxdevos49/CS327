all: pacman

pacman: main.cpp lib/Game.cpp lib/SceneManager.cpp lib/Renderer.cpp scenes/GameScene.cpp scenes/TitleScene.cpp scenes/DebugScene.cpp scenes/MenuScene.cpp
	g++ -Wall -Werror -ggdb2 -O0 main.cpp lib/Game.cpp lib/SceneManager.cpp lib/Renderer.cpp scenes/GameScene.cpp scenes/TitleScene.cpp scenes/DebugScene.cpp scenes/MenuScene.cpp -o pacman -lm -lncurses

clean:
	rm -rf ./pacman *.dSYM .vscode

	