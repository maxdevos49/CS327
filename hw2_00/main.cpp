#include <iostream>
#include <cstdlib>

#include <ncurses.h>

#include "headers/game.h"
#include "headers/render.h"

int main(int argc, char const *argv[])
{

    srand(1);

    init_terminal();

    while (true)
    {

        draw_map();

        if (getch() != ERR)
            break;
    }

    endwin();

    return 0;
}
