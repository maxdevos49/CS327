#include <iostream>
#include <ncurses.h>

#include "TitleScene.h"

TitleScene::TitleScene()
{
    key = (char*)"TitleScene";

    width = 0;
}

void TitleScene::create()
{
    //init stuff here
}

void TitleScene::update()
{
    //update stuff here
}

void TitleScene::render()
{
    renderer->clear_window();

    int i, j;

    (width <= renderer->screen_width) ? width++ : width = 0;
    
    for(i = 5; i < renderer->screen_height/3 + 1; i++)
    {
        renderer->draw_rect_color(0,i * 3,width + i,1, BG_YELLOW);

        renderer->draw_rect_color(0,i * 3 + 2,width + i,1,BG_BLUE);
    }

    //title
    for(i = 0; i < 8; i++)
        for(j = 0; j < 63; j++)
            if(title_array[i][j] == '$')
            {
                attron(COLOR_PAIR(BG_YELLOW));
                mvaddch(3 + i, 10 + j, ' ');
                attroff(COLOR_PAIR(BG_YELLOW));

            } else {
                attron(COLOR_PAIR(FG_YELLOW));
                mvaddch(3 + i, 10 + j, title_array[i][j]);
                attroff(COLOR_PAIR(FG_YELLOW));
            }

    //press any key to continue
    renderer->draw_rect_color(renderer->screen_width/2 - 12, 39, 24, 3, BG_RED);
    attron(A_BLINK | A_BOLD);
    mvprintw(40,renderer->screen_width/2 - 11, "Press any key to play");
    attroff(A_BLINK | A_BOLD);

    
}

void TitleScene::input(int givenKey)
{
    switch(givenKey)
    {
        case 'm':
            manager->start_scene((char*)"MenuScene");
        break;
        case 's': 
            manager->start_scene((char*)"GameScene");
            break;
         case 'D':
            manager->launch_scene((char*)"DebugScene");
            break;
    }
}  