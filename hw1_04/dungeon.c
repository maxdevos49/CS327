#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
 * Dungeon Crawler Game
 * --save, -s
 * --load, -l
 * --seed <SEED>
 * --nummon <NPC COUNT>
 * 
 * Tested on MacOs and Linux
 * */
int main(int argc, char *argv[])
{
    action_t action;
    dungeon_t d;
    d.seed = 0;
    d.npc_count = 10;
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
        printf("loading...\n");
        load_dungeon(&d);
        break;
    case save:
        printf("saving...\n");
        generate(&d);
        save_dungeon(&d);
        break;
    case load_save:
        printf("Loading and saving...\n");
        load_dungeon(&d);
        save_dungeon(&d);
        break;
    default:
        generate(&d);
        break;
    }

    add_monsters(&d);
    distance_maps(&d);

    game_loop(&d);        

    //draw loosing scene
    free_dungeon(&d);
    printf("\n\n\n\n\n\n\n\n\t\t\tYou Lose :(\n\n\n\n\n\n\n\n\n\n\n");

    return 0;
}
