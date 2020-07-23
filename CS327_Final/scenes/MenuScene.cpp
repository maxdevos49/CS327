#include <iostream>
#include "MenuScene.h"

MenuScene::MenuScene()
{
    key = (char*)"MenuScene";
    selector = 0;
}

void MenuScene::input(int givenKey)
{
    switch(givenKey)
    {
        case 'D':
            manager->launch_scene((char*)"DebugScene");
            break;
        case 'h':
            selector++;
            if(selector > manager->scenes.size())
                selector = 0;
            break;
        case 'y':
            selector--;
            if(selector < 0)
                selector = manager->scenes.size();
            break;
        case 'e':
            manager->start_scene(manager->scenes[selector]->key);
    }
}
void MenuScene::render()
{
    int frame_W = 60;
    int frame_H = 30;
    renderer->clear_window();
    renderer->draw_rect_color(renderer->screen_width/2 - frame_W/2, 3, frame_W, frame_H, BG_RED);

    attron(COLOR_PAIR(BG_RED));
    mvprintw(5,renderer->screen_width/2 - 7, "Select a scene");
    

    int i;
    for(i = 0; i < manager->scenes.size(); i++)
    {
        if(selector == i){
            attron(A_BOLD);
            mvprintw(7 + i, renderer->screen_width/2 - 7, "-%s",manager->scenes[i]->key);
            attroff(A_BOLD);
        }else{
            mvprintw(7 + i, renderer->screen_width/2 - 7, "-%s",manager->scenes[i]->key);
        }
    }
    attroff(COLOR_PAIR(BG_RED));


}