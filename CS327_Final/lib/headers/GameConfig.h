#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <vector>

#include "Scene.h"

class GameConfig
{
public:
    /**
     * The width of the window. Must be positive and non zero
     * */
    int screen_width;

    /**
     * The height of the window. Must be positive and non zero
     * */
    int screen_height;

    /**
     * The list of scenes in the Game
     * */
    std::vector<Scene*> scene_array;

    /**
     * Indicates whether debug information should be printed
     * */
    bool debug;

    /**
     * Determines if the game autostarts
     * */
    bool auto_start;

    /**
     *  Default starting scene key
     * */
    char* default_scene;
};

#endif