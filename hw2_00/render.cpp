#include <iostream>
#include "ncurses.h"

#include "headers/game.h"
#include "headers/render.h"

void init_terminal()
{
    initscr();
    curs_set(0);
    start_color();

    keypad(stdscr, 1);
    cbreak();
    timeout(0);

    //backgrounds
    init_pair(BG_RED, COLOR_WHITE, COLOR_RED);
    init_pair(BG_YELLOW, COLOR_WHITE, COLOR_YELLOW);
    init_pair(BG_GREEN, COLOR_WHITE, COLOR_GREEN);
    init_pair(BG_BLUE, COLOR_WHITE, COLOR_BLUE);
    init_pair(BG_BLACK, COLOR_WHITE, 234);
    init_pair(BG_WHITE, COLOR_BLACK, COLOR_WHITE);
}

void draw_map()
{
    int x, y;
    for (y = 0; y < MAP_HEIGHT; y++)
    {

        if (rand() % 2)
        {
            attron(COLOR_PAIR(BG_BLUE));
            for (x = 0; x < MAP_WIDTH; x++)
            {
                mvaddch(y, x, ' ');
            }
            attroff(COLOR_PAIR(BG_BLUE));
        }
        else
        {
            attron(COLOR_PAIR(BG_YELLOW));
            for (x = 0; x < MAP_WIDTH; x++)
            {
                mvaddch(y, x, ' ');
            }
            attroff(COLOR_PAIR(BG_YELLOW));
        }
    }
    refresh();
}

void draw_title_screen()
{
    
}