#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ncurses.h>

#include "headers/dungeon.h"
#include "headers/generator.h"
#include "headers/load_save.h"
#include "headers/render.h"
#include "headers/path_finding.h"

void usage(char *name)
{
    fprintf(stderr, "Usage: %s [--seed <seed>] [-l|--load ]\n"
                    "          [-s|--save ] [--nummon <npc count>] \n",
            name);
    exit(-1);
}

/**
 * RougeLike
 * --save, -s
 * --load, -l
 * --seed <SEED>
 * --nummon <NPC COUNT>
 * */
int main(int argc, char *argv[])
{
    action_t action;
    Dungeon d;
    d.seed = 0;
    d.npc_count = 10;
    d.hud.status = (char *)"";
    int i;

    for (i = 1; i < argc; i++) //check for flags
    {
        if (!strcmp(argv[i], "--save") || !strcmp(argv[i], "-s"))
        {
            action = (action != load) ? save : load_save;
        }
        else if (!strcmp(argv[i], "--load") || !strcmp(argv[i], "-l"))
        {
            action = (action != save) ? load : load_save;
        }
        else if (!strcmp(argv[i], "--seed") && (i + 1 < argc))
        {
            i++;
            d.seed = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "--nummon") && (i + 1 < argc))
        {
            i++;
            d.npc_count = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-sl") || !strcmp(argv[i], "-ls"))
        {
            action = load_save;
        }
        else
        {
            usage(argv[0]);
        }
    }

    switch (action)
    {
    case load:
        load_dungeon(&d);
        break;
    case save:
        generate(&d);
        save_dungeon(&d);
        break;
    case load_save:
        load_dungeon(&d);
        save_dungeon(&d);
        break;
    default:
        generate(&d);
        break;
    }
    init_terminal();
    game_loop(&d);

    if (d.mode == m_quit)
        show_quit(&d);
    else if (d.mode == m_killed)
        show_death(&d);
        
    free_dungeon(&d);
    endwin();

    return 0;
}
