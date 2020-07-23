#include <iostream>

#include "headers/Game.h"
#include "headers/GameConfig.h"

/**
 * Constructs a Game based on a Game config that is supplied.
 * */
Game::Game(GameConfig config, State *givenData) : 
    manager(config.scene_array),
    renderer(config.screen_width, config.screen_height)
{

    //properties
    screen_width = config.screen_width;
    screen_height = config.screen_height;
    
    default_scene = config.default_scene;
    debug = config.debug;
    running = false;

    data = givenData;

    //prepare scenes
    int i;
    for(i = 0; i < manager.scenes.size(); i++)
    {
        manager.scenes[i]->add_scene_manager(&manager);
        manager.scenes[i]->add_renderer(&renderer);
        manager.scenes[i]->add_state_manager(data);
        manager.scenes[i]->state = ready;
    }

    if (config.auto_start)
    {
        running = true;
        manager.start_scene(default_scene);
    }
}

/**
 * Starts a Game if the auto start property was not set to true in the config
 * */
void Game::start_game()
{
    if (!running)
    {
        running = true;
        manager.start_scene(default_scene);
    }
}