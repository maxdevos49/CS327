#include <iostream>
#include <cstdlib>

#include "lib/headers/Game.h"
#include "lib/headers/GameConfig.h"

#include "scenes/GameScene.h"
#include "scenes/TitleScene.h"
#include "scenes/DebugScene.h"
#include "scenes/MenuScene.h"

#include "state.h"

int main(int argc, char const *argv[])
{

    //make game config
    GameConfig config;

    config.screen_width = 84;
    config.screen_height = 46;
    config.debug = false;
    config.auto_start = true;
    config.default_scene = (char*)"TitleScene";

    //init scenes
    GameScene gamescene;
    TitleScene titlescene;
    DebugScene debugscene;
    MenuScene menuscene;

    //add scenes to game config
    config.scene_array.push_back(&gamescene);
    config.scene_array.push_back(&titlescene);
    config.scene_array.push_back(&debugscene);
    config.scene_array.push_back(&menuscene);

    //Pass in a state manager
    State data;

    //start game
    Game game(config, &data);

    return 0;
}
