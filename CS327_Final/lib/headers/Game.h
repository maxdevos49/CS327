#ifndef GAME_H
#define GAME_H

#include "SceneManager.h"
#include "GameConfig.h"
#include "Renderer.h"
#include "../../state.h"

class Game
{
private:
    SceneManager manager;
    Renderer renderer;
    State *data;

public:
    int screen_width;
    int screen_height;
    bool running;
    bool debug;

    char* default_scene;

    Game(GameConfig config, State *givenData);

    void start_game();
};

#endif