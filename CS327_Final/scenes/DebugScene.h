#ifndef DEBUGSCENE_H
#define DEBUGSCENE_H

#include "../lib/headers/Scene.h"

class DebugScene : public Scene
{
    public:
    DebugScene();

    void input(int givenKey);
    void render();
};

#endif