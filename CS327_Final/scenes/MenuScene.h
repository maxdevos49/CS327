#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "../lib/headers/Scene.h"

class MenuScene : public Scene
{
    public:
    int selector;
    MenuScene();

    void input(int givenKey);
    void render();
};

#endif