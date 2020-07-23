#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "../lib/headers/Scene.h"

class GameScene : public Scene
{
    public:
    GameScene();

    void input(int givenKey);
    void create();
    void update();
    void render();
};

#endif