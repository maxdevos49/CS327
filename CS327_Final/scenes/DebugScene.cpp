#include <iostream>
#include "DebugScene.h"

DebugScene::DebugScene()
{
    key = (char *)"DebugScene";
}


void DebugScene::render()
{
    //background    
    int frame_W = 40;
    int frame_H = 25;
    int i, j;
    renderer->draw_rect_color(renderer->screen_width/2 - frame_W/2, 5, frame_W, frame_H, BG_BLUE);

    renderer->draw_rect_color(renderer->screen_width/2 - frame_W/2 + 1, 6,frame_W - 2, 3, BG_RED);
    mvprintw(7, renderer->screen_width/2 - 3, "Debug");

    attron(COLOR_PAIR(BG_BLUE));
    mvprintw(10, renderer->screen_width/2 - 10, "FPS Setting: %d", 60);
    mvprintw(12, renderer->screen_width/2 - 10, "Screen Width: %d", renderer->screen_width);
    mvprintw(13, renderer->screen_width/2 - 10, "Screen Height: %d", renderer->screen_height);

    mvprintw(15, renderer->screen_width/2 - 10, "Active Scenes: %d",manager->active_scene.size());

    mvprintw(17, renderer->screen_width/2 - 10, "Active Scene List:");

    for(i = 0; i < manager->active_scene.size(); i++){
        mvprintw(18 + i, renderer->screen_width/2 - 10, "-%s", manager->active_scene[i]->key);
    }

    mvprintw(20 + i, renderer->screen_width/2 - 10, "Available Scene List:");

    for(j = 0; j < manager->scenes.size(); j++){
        mvprintw(21 +  i + j, renderer->screen_width/2 - 10, "-%s", manager->scenes[j]->key);
    }

    mvprintw(23 + i + j, renderer->screen_width/2 - 16, "Press the 'd' key to close debug");
    attroff(COLOR_PAIR(BG_BLUE));
    
}

void DebugScene::input(int givenKey)
{
    switch(givenKey)
    {
        case 'm':
            manager->start_scene((char*)"MenuScene");
            break;
        case 'd':
            manager->shutdown_scene((char*)"DebugScene");
            break;      
    }  
}