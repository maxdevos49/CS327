#include <iostream>
#include <ncurses.h>

#include "GameScene.h"
#include "../state.h"

GameScene::GameScene()
{
    key = (char*)"GameScene";
}

void GameScene::create()
{
    data->score = 0;
    data->max_score = 0;
}

void GameScene::update()
{
    //update stuff here
}

void GameScene::render()
{
    renderer->clear_window();

    int i, j,x = renderer->screen_width/2 - data->map_width, y = 10, width = 2;
    //map
    for(i = 0; i < data->map_height; i++)
    {
        for(j = 0; j < data->map_width; j++)
        {
            switch(map[i][j])
            {
                case 0:
                    renderer->draw_rect_char_color(j * width + x,i + y,width,1, '.', BG_BLACK);
                    break;
                case 1:
                    renderer->draw_rect_color(j * width + x,i + y,width,1, BG_BLUE);
                    break;
                case 2:
                case 3:
                    renderer->draw_rect_color(j * width + x,i + y,width,1, BG_BLACK);
                    break;
                case 4:
                    attron(A_BLINK);
                    renderer->draw_rect_char_color(j * width + x,i + y,width,1, '+', FG_WHITE);
                    attroff(A_BLINK);
                    break;
                case 5:
                    renderer->draw_rect_color(j * width + x,i + y,width,1, BG_WHITE);
                    break;
            }
        }
    }

}

void GameScene::input(int givenKey)
{
    switch(givenKey)
    {
        case 'm':
            manager->start_scene((char*)"MenuScene");
            break;
        case 's':
            manager->start_scene((char*)"TitleScene");
            break;
        case 'D':
            manager->launch_scene((char*)"DebugScene");
            break;
    }
}