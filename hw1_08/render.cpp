#include <ncurses.h>
#include <cstdlib>
#include <cmath>

#include "headers/dungeon.h"
#include "headers/render.h"

#define GROUND 233

const char you_quit[5][40] = {
    {'_', '_', ' ', ' ', '_', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', '_', '_', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', ' ', '_', '_', ' ', ' ', ' '},
    {'\\', ' ', '\\', '/', ' ', '/', '_', '_', '_', ' ', ' ', '_', '_', ' ', ' ', '_', '_', ' ', ' ', ' ', '/', ' ', '_', '_', ' ', '\\', '_', '_', ' ', ' ', '_', '_', '(', '_', ')', ' ', '/', '_', ' ', ' '},
    {' ', '\\', ' ', ' ', '/', ' ', '_', '_', ' ', '\\', '/', ' ', '/', ' ', '/', ' ', '/', ' ', ' ', '/', ' ', '/', ' ', '/', ' ', '/', ' ', '/', ' ', '/', ' ', '/', ' ', '/', ' ', '_', '_', '/', ' ', ' '},
    {' ', '/', ' ', '/', ' ', '/', '_', '/', ' ', '/', ' ', '/', '_', '/', ' ', '/', ' ', ' ', '/', ' ', '/', '_', '/', ' ', '/', ' ', '/', '_', '/', ' ', '/', ' ', '/', ' ', '/', '_', ' ', ' ', ' ', ' '},
    {'/', '_', '/', '\\', '_', '_', '_', '_', '/', '\\', '_', '_', ',', '_', '/', ' ', ' ', ' ', '\\', '_', '_', '_', '\\', '_', '\\', '_', '_', ',', '_', '/', '_', '/', '\\', '_', '_', '/', ' ', ' ', ' ', ' '},
};

const char *you_win =
    "$$\\     $$\\                         $$\\      $$\\ $$\\           \n"
    "\\$$\\   $$  |                        $$ | $\\  $$ |\\__|          \n"
    " \\$$\\ $$  /$$$$$$\\  $$\\   $$\\       $$ |$$$\\ $$ |$$\\ $$$$$$$\\  \n"
    "  \\$$$$  /$$  __$$\\ $$ |  $$ |      $$ $$ $$\\$$ |$$ |$$  __$$\\ \n"
    "   \\$$  / $$ /  $$ |$$ |  $$ |      $$$$  _$$$$ |$$ |$$ |  $$ |\n"
    "    $$ |  $$ |  $$ |$$ |  $$ |      $$$  / \\$$$ |$$ |$$ |  $$ |\n"
    "    $$ |  \\$$$$$$  |\\$$$$$$  |      $$  /   \\$$ |$$ |$$ |  $$ |\n"
    "    \\__|   \\______/  \\______/       \\__/     \\__|\\__|\\__|  \\__|\n"
    "                                                               \n";

const char you_died[5][38] = {
    {' ', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', '_', ' '},
    {'|', ' ', '|', ' ', ' ', ' ', ' ', ' ', '/', ' ', '/', '_', '_', '_', ' ', '_', '_', '_', '_', '_', '_', '/', ' ', '/', '_', '_', '_', '_', ' ', ' ', '_', '_', '_', '_', '/', ' ', '/', ' '},
    {'|', ' ', '|', ' ', '/', '|', ' ', '/', ' ', '/', ' ', '_', '_', ' ', '`', '/', ' ', '_', '_', '_', '/', ' ', '_', '_', '/', ' ', '_', ' ', '\\', '/', ' ', '_', '_', ' ', ' ', '/', ' ', ' '},
    {'|', ' ', '|', '/', ' ', '|', '/', ' ', '/', ' ', '/', '_', '/', ' ', '(', '_', '_', ' ', ' ', ')', ' ', '/', '_', '/', ' ', ' ', '_', '_', '/', ' ', '/', '_', '/', ' ', '/', ' ', ' ', ' '},
    {'|', '_', '_', '/', '|', '_', '_', '/', '\\', '_', '_', ',', '_', '/', '_', '_', '_', '_', '/', '\\', '_', '_', '/', '\\', '_', '_', '_', '/', '\\', '_', '_', ',', '_', '/', ' ', ' ', ' ', ' '},
};

enum color
{
    //backgrounds,
    BG_RED = 1,
    BG_ORANGE,
    BG_YELLOW,
    BG_GREEN,
    BG_BLUE,
    BG_PURPLE,
    BG_WHITE,
    BG_BLACK,

    BG_DARKGREEN,
    BG_LIGHTGREEN,

    BG_DULLMAGENTA,

    //colors
    C_RED,
    C_ORANGE,
    C_YELLOW,
    C_GREEN,
    C_BLUE,
    C_PURPLE,
    C_WHITE,
    C_BLACK,
    C_GREY,

    //Greys
    BG_GREY1,
    BG_GREY2,
    BG_GREY3,
    BG_GREY4,
    BG_GREY5,
    BG_GREY6,
    BG_GREY7,

    BG_DARKGROUND,
    BG_DARKISHGROUND,
    BG_DARKROCK

};

void init_terminal(void)
{
    initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, 1);
    start_color();

    //backgrounds
    init_pair(BG_RED, COLOR_WHITE, COLOR_RED);
    init_pair(BG_YELLOW, COLOR_WHITE, COLOR_YELLOW);
    init_pair(BG_GREEN, COLOR_WHITE, COLOR_GREEN);

    init_pair(BG_DARKGREEN, COLOR_WHITE, 118);
    init_pair(BG_LIGHTGREEN, COLOR_WHITE, 10);

    init_pair(BG_DULLMAGENTA, COLOR_WHITE, 126);

    init_pair(BG_BLUE, COLOR_WHITE, COLOR_BLUE);
    init_pair(BG_BLACK, COLOR_WHITE, 234);
    init_pair(BG_WHITE, COLOR_BLACK, COLOR_WHITE);

    init_pair(BG_GREY1, COLOR_WHITE, 241); //ground 1-7
    init_pair(BG_GREY2, COLOR_WHITE, 240);
    init_pair(BG_GREY3, COLOR_WHITE, 239);
    init_pair(BG_GREY4, COLOR_WHITE, 238);
    init_pair(BG_GREY5, COLOR_WHITE, 237);
    init_pair(BG_GREY6, COLOR_WHITE, 236);
    init_pair(BG_GREY7, COLOR_WHITE, 235);

    //foregrounds
    init_pair(C_YELLOW, 11, GROUND); //stairs
    init_pair(C_WHITE, 248, GROUND); //ground
    init_pair(C_BLUE, 40, GROUND);   //player
    init_pair(C_GREY, 244, GROUND);  //player

    init_pair(BG_DARKGROUND, 239, GROUND - 1);    //dark ground
    init_pair(BG_DARKISHGROUND, 239, GROUND + 1); //dark ground
    init_pair(BG_DARKROCK, 0, 0);                 //black
}

void draw_map(Dungeon *d)
{

    update_hud(d);

    uint8_t i, j;

    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            if (d->explored[i][j] == c_nothin)
            {
                attron(COLOR_PAIR(BG_DARKROCK));
                mvaddch(i + 1, j, ' ');
            }
            else
            {
                if (d->map[i][j] != c_rock)
                    attron(COLOR_PAIR(BG_DARKGROUND));
                else
                    attron(COLOR_PAIR(BG_DARKISHGROUND));

                mvaddch(i + 1, j, d->explored[i][j]);
            }
        }
    }

    for (i = d->player.y - RADIUS; i < d->player.y + RADIUS; i++)
    {
        for (j = d->player.x - RADIUS; j < d->player.x + RADIUS; j++)
        {
            int distance = sqrt(pow(d->player.x - j, 2) + pow(d->player.y - i, 2));

            if (distance < RADIUS)
            {
                d->explored[i][j] = d->map[i][j];

                terrain_t terrain = d->map[i][j];
                npc_t npc;
                if (d->characters[i][j] != NULL)
                    npc = (npc_t)((d->characters[i][j]->type) ? n_player : d->characters[i][j]->behavior + 1);

                int percent = (distance * 100) / RADIUS;
                //background color
                uint8_t hardness = d->hardness[i][j];
                if (hardness == 0)
                {
                    if (percent < 50)
                    {
                        attron(A_BOLD);
                        attron(COLOR_PAIR(C_WHITE));
                    }
                    else
                    {
                        attron(COLOR_PAIR(C_GREY));
                    }
                }
                else if (hardness == 255)
                    attron(COLOR_PAIR(BG_BLACK));
                else if (hardness < 36)
                    attron(COLOR_PAIR(BG_GREY1));
                else if (hardness < 72)
                    attron(COLOR_PAIR(BG_GREY2));
                else if (hardness < 108)
                    attron(COLOR_PAIR(BG_GREY3));
                else if (hardness < 144)
                    attron(COLOR_PAIR(BG_GREY4));
                else if (hardness < 180)
                    attron(COLOR_PAIR(BG_GREY5));
                else if (hardness < 216)
                    attron(COLOR_PAIR(BG_GREY6));
                else
                    attron(COLOR_PAIR(BG_GREY7));

                if (terrain == c_d_stairs || terrain == c_u_stairs)
                    attron(COLOR_PAIR(C_YELLOW));

                //character color
                if (d->characters[i][j] != NULL)
                    if (npc == n_player) //change to switch later for npcs
                        attron(COLOR_PAIR(C_BLUE));

                if (d->characters[i][j] == NULL)
                {
                    mvaddch(i + 1, j, d->map[i][j]);
                }
                else
                {
                    i += 1;
                    //this will go away soon
                    switch (npc)
                    {
                    case n_player:
                        mvaddch(i, j, '@');
                        break;
                    default:
                        mvprintw(i, j, "%x", npc - 1);
                        break;
                    }
                    i -= 1;
                }
                attroff(A_BOLD);
            }
        }
    }
    refresh();
    return;
}

void full_map(Dungeon *d)
{
    update_hud(d);

    uint8_t i, j;

    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            terrain_t terrain = d->map[i][j];
            npc_t npc;
            if (d->characters[i][j] != NULL)
                npc = (npc_t)((d->characters[i][j]->type) ? n_player : d->characters[i][j]->behavior + 1);

            //background color
            uint8_t hardness = d->hardness[i][j];
            if (hardness == 0)
                attron(COLOR_PAIR(C_WHITE));
            else if (hardness == 255)
                attron(COLOR_PAIR(BG_BLACK));
            else if (hardness < 36)
                attron(COLOR_PAIR(BG_GREY1));
            else if (hardness < 72)
                attron(COLOR_PAIR(BG_GREY2));
            else if (hardness < 108)
                attron(COLOR_PAIR(BG_GREY3));
            else if (hardness < 144)
                attron(COLOR_PAIR(BG_GREY4));
            else if (hardness < 180)
                attron(COLOR_PAIR(BG_GREY5));
            else if (hardness < 216)
                attron(COLOR_PAIR(BG_GREY6));
            else
                attron(COLOR_PAIR(BG_GREY7));

            if (terrain == c_d_stairs || terrain == c_u_stairs)
                attron(COLOR_PAIR(C_YELLOW));

            //character color
            if (d->characters[i][j] != NULL)
                if (npc == n_player) //change to switch later for npcs
                    attron(COLOR_PAIR(C_BLUE));

            if (d->characters[i][j] == NULL)
            {
                mvaddch(i + 1, j, d->map[i][j]);
            }
            else
            {
                i += 1;
                //this will go away soon
                switch (npc)
                {
                case n_player:
                    mvaddch(i, j, '@');
                    break;
                default:
                    mvprintw(i, j, "%x", npc - 1);
                    break;
                }
                i -= 1;
            }
        }
    }
}

void draw_hardness(Dungeon *d)
{
    printf("\nHardness Map\n");
    uint8_t i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
            printf("%02x", d->hardness[i][j]);
        printf("\n");
    }
    return;
}

void update_hud(Dungeon *d)
{
    uint8_t i, j;
    //apply color
    attron(COLOR_PAIR(BG_WHITE));

    //clear top hud
    for (i = 0; i < MAP_WIDTH; i++)
        mvaddch(0, i, ' ');

    //clear bottom hud
    for (i = MAP_HEIGHT + 1; i < MAP_HEIGHT + 3; i++)
        for (j = 0; j < MAP_WIDTH; j++)
            mvaddch(i, j, ' ');

    //status
    mvprintw(0, 0, "%s", d->hud.status);

    //seed
    mvprintw(MAP_HEIGHT + 1, 0, "Seed: %d", d->seed);
    //key press
    mvprintw(MAP_HEIGHT + 2, MAP_WIDTH / 2 + 31, "Key: %d", d->hud.key);
    //kills
    mvprintw(MAP_HEIGHT + 2, 13, "Kills: %2d", d->characters[d->player.y][d->player.x]->kills);
    //Turns
    mvprintw(MAP_HEIGHT + 2, 25, "Turns: %4d", d->characters[d->player.y][d->player.x]->turns);
    //position
    mvprintw(MAP_HEIGHT + 2, 0, "X: %2d, Y: %2d", d->player.x, d->player.y);

    refresh();

    return;
}

void clear_screen(void)
{
    uint8_t i, j;
    attron(COLOR_PAIR(BG_WHITE));
    for (i = 0; i < MAP_HEIGHT + 3; i++)
        for (j = 0; j < MAP_WIDTH; j++)
            mvaddch(i, j, ' ');
}

void draw_monster_list(Dungeon *d, int8_t direction)
{
    static uint16_t position;
    static uint16_t length;
    int vertical_position;
    uint8_t i, j, y = 1;
    uint8_t offset = 49;
    uint16_t rel_length = 0;

    //init list mode
    if (d->mode != m_monster_list)
    {
        d->mode = m_monster_list;
        position = 1;
        length = 0;

        //get total
        for (i = 0; i < MAP_HEIGHT; i++)
            for (j = 0; j < MAP_WIDTH; j++)
                if (d->characters[i][j] != NULL && !d->characters[i][j]->type)
                    length++;

        //draw background
        attron(COLOR_PAIR(BG_GREY6));
        for (i = 0; i < MAP_HEIGHT + 3; i++)
            for (j = offset + 1; j < MAP_WIDTH; j++)
                mvaddch(i, j, ' ');

        //list heading
        mvprintw(0, offset + 20, "Total: %3d", length);
        mvprintw(0, offset + 4, "Monster List");
    }
    else
    {
        position += direction;
        //enforce scroll bounds
        if (position > length - 21)
            position -= direction;
    }

    //list
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            if (d->characters[i][j] != NULL && !d->characters[i][j]->type)
            {
                rel_length++;
                if (rel_length < position) //check if we have the ones we desire
                    continue;

                if (y > 22) //have enough for visable list so lets get outta here
                    break;

                //print directions
                (!(y % 2)) ? attron(COLOR_PAIR(BG_WHITE)) : attron(COLOR_PAIR(BG_GREY1));
                mvprintw(y, offset + 4, "#%3d, %x, %2d %s %2d %s ", rel_length, d->characters[i][j]->behavior, abs(d->player.x - j), (d->player.x > j) ? "West" : "East", abs(d->player.y - i), (d->player.y < i) ? "South" : "North");
                y++;
            }
        }
    }

    //calculate scroll bar position
    if (length > 0)
    {
        vertical_position = ((position)*21) / (length - 21);

        //draw scrolltrack
        attron(COLOR_PAIR(BG_GREY3));
        for (i = 1; i < MAP_HEIGHT + 2; i++)
            mvaddch(i, offset + 2, ' ');

        //scrollbar
        attron(COLOR_PAIR(BG_BLACK));
        for (i = vertical_position; i < (21 / length) + vertical_position; i++)
            mvaddch(i + 1, offset + 2, ' ');

        // default size for scroll bar
        if (i == vertical_position)
            mvaddch(vertical_position + 1, offset + 2, ' ');
    }
    refresh();
    return;
}

void monster_list_control(Dungeon *d)
{
    draw_monster_list(d, 0);
    while (1)
    {
        int character = getch();

        switch (character)
        {
        case 109: //leave list
        case 27:
            d->mode = m_play;
            goto exit_monster_list;
            break;
        case 81:
            d->mode = m_quit;
            goto exit_monster_list;
            break;
        case 258: //down
            draw_monster_list(d, 1);
            break;
        case 259: //up
            draw_monster_list(d, -1);
            break;
        }
    }

exit_monster_list:
    draw_map(d);
    return;
}

void show_quit(Dungeon *d)
{
    clear_screen();
    cbreak();
    timeout(500);
    uint8_t grid[24][80];
    uint8_t next[24][80];
    uint8_t i, j;

    attron(COLOR_PAIR(BG_WHITE));
    for (i = 0; i < MAP_HEIGHT + 3; i++)
        for (j = 0; j < MAP_WIDTH; j++)
            grid[i][j] = rand() % 2;

    while (1)
    {
        // check grid
        for (i = 0; i < MAP_HEIGHT + 3; i++)
        {
            for (j = 0; j < MAP_WIDTH; j++)
            {

                uint8_t state = grid[i][j];
                uint8_t neighbors = 0;
                int8_t k, l;

                for (k = -1; k < 2; k++)
                {
                    for (l = -1; l < 2; l++)
                    {
                        uint8_t col = (j + l + MAP_WIDTH) % MAP_WIDTH;
                        uint8_t row = (i + k + MAP_HEIGHT + 3) % (MAP_HEIGHT + 3);
                        neighbors += grid[row][col];
                    }
                }

                neighbors -= state;

                if (state == 0 && neighbors == 3)
                    next[i][j] = 1;
                else if (state == 1 && (neighbors < 2 || neighbors > 3))
                    next[i][j] = 0;
                else
                    next[i][j] = state;
            }
        }

        for (i = 0; i < MAP_HEIGHT + 3; i++)
            for (j = 0; j < MAP_WIDTH; j++)
                grid[i][j] = next[i][j];

        //draw
        for (i = 0; i < MAP_HEIGHT + 3; i++)
            for (j = 0; j < MAP_WIDTH; j++)
                if (grid[i][j] == 1)
                {
                    attron(COLOR_PAIR(BG_LIGHTGREEN));
                    mvaddch(i, j, ' ');
                }
                else
                {
                    attron(COLOR_PAIR(BG_DARKGREEN));
                    mvaddch(i, j, ' ');
                }

        //message background
        for (i = 0; i < 7; i++)
        {
            for (j = 0; j < 40; j++)
            {
                attron(COLOR_PAIR(BG_WHITE));
                mvaddch(i + (MAP_HEIGHT + 3) / 2 - 4, j + MAP_WIDTH / 2 - 40 / 2, ' ');
            }
        }

        //message
        for (i = 0; i < 5; i++)
        {
            for (j = 0; j < 38; j++)
            {
                attron(COLOR_PAIR(BG_BLACK));
                mvaddch(i + MAP_HEIGHT / 2 - 1, j + MAP_WIDTH / 2 - 38 / 2, you_quit[i][j]);
            }
        }

        attron(COLOR_PAIR(BG_RED));
        mvprintw(20, MAP_WIDTH / 2 - 11, "Press any key to quit");

        refresh();
        if (getch() != ERR)
            break;
    }
}

void show_death(Dungeon *d)
{
    clear_screen();
    cbreak();
    timeout(500);
    uint8_t grid[24][80];
    uint8_t next[24][80];
    uint8_t i, j;

    attron(COLOR_PAIR(BG_WHITE));
    for (i = 0; i < MAP_HEIGHT + 3; i++)
        for (j = 0; j < MAP_WIDTH; j++)
            grid[i][j] = rand() % 2;

    while (1)
    {
        // check grid
        for (i = 0; i < MAP_HEIGHT + 3; i++)
        {
            for (j = 0; j < MAP_WIDTH; j++)
            {

                uint8_t state = grid[i][j];
                uint8_t neighbors = 0;
                int8_t k, l;

                for (k = -1; k < 2; k++)
                {
                    for (l = -1; l < 2; l++)
                    {
                        uint8_t col = (j + l + MAP_WIDTH) % MAP_WIDTH;
                        uint8_t row = (i + k + MAP_HEIGHT + 3) % (MAP_HEIGHT + 3);
                        neighbors += grid[row][col];
                    }
                }

                neighbors -= state;

                if (state == 0 && neighbors == 3)
                    next[i][j] = 1;
                else if (state == 1 && (neighbors < 2 || neighbors > 3))
                    next[i][j] = 0;
                else
                    next[i][j] = state;
            }
        }

        for (i = 0; i < MAP_HEIGHT + 3; i++)
            for (j = 0; j < MAP_WIDTH; j++)
                grid[i][j] = next[i][j];

        //draw
        for (i = 0; i < MAP_HEIGHT + 3; i++)
            for (j = 0; j < MAP_WIDTH; j++)
                if (grid[i][j] == 1)
                {
                    attron(COLOR_PAIR(BG_LIGHTGREEN));
                    mvaddch(i, j, ' ');
                }
                else
                {
                    attron(COLOR_PAIR(BG_DARKGREEN));
                    mvaddch(i, j, ' ');
                }

        //message background
        for (i = 0; i < 9; i++)
        {
            for (j = 0; j < 39; j++)
            {
                attron(COLOR_PAIR(BG_WHITE));
                mvaddch(i + (MAP_HEIGHT + 3) / 2 - 4, j + MAP_WIDTH / 2 - 40 / 2, ' ');
            }
        }

        //message
        for (i = 0; i < 5; i++)
        {
            for (j = 0; j < 37; j++)
            {
                attron(COLOR_PAIR(BG_BLACK));
                mvaddch(i + MAP_HEIGHT / 2 - 1, j + MAP_WIDTH / 2 - 38 / 2, you_died[i][j]);
            }
        }

        attron(COLOR_PAIR(BG_WHITE));
        attron(A_BLINK);
        mvprintw(15, MAP_WIDTH / 2 - 11 / 2, "Kills: %3d", d->characters[d->player.y][d->player.x]->kills);
        mvprintw(16, MAP_WIDTH / 2 - 11 / 2, "Turns: %3d", d->characters[d->player.y][d->player.x]->turns);
        attroff(A_BLINK);

        attron(COLOR_PAIR(BG_RED));
        mvprintw(20, MAP_WIDTH / 2 - 11, "Press any key to quit");

        refresh();
        if (getch() != ERR)
            break;
    }
}

//not used yet
void show_win(Dungeon *d)
{
    clear_screen();
    cbreak();
    timeout(500);
    uint8_t grid[24][80];
    uint8_t next[24][80];
    uint8_t i, j;

    attron(COLOR_PAIR(BG_WHITE));
    for (i = 0; i < MAP_HEIGHT + 3; i++)
        for (j = 0; j < MAP_WIDTH; j++)
            grid[i][j] = rand() % 2;

    while (1)
    {
        // check grid
        for (i = 0; i < MAP_HEIGHT + 3; i++)
        {
            for (j = 0; j < MAP_WIDTH; j++)
            {

                uint8_t state = grid[i][j];
                uint8_t neighbors = 0;
                int8_t k, l;

                for (k = -1; k < 2; k++)
                {
                    for (l = -1; l < 2; l++)
                    {
                        uint8_t col = (j + l + MAP_WIDTH) % MAP_WIDTH;
                        uint8_t row = (i + k + MAP_HEIGHT + 3) % (MAP_HEIGHT + 3);
                        neighbors += grid[row][col];
                    }
                }

                neighbors -= state;

                if (state == 0 && neighbors == 3)
                    next[i][j] = 1;
                else if (state == 1 && (neighbors < 2 || neighbors > 3))
                    next[i][j] = 0;
                else
                    next[i][j] = state;
            }
        }

        for (i = 0; i < MAP_HEIGHT + 3; i++)
            for (j = 0; j < MAP_WIDTH; j++)
                grid[i][j] = next[i][j];

        //draw
        for (i = 0; i < MAP_HEIGHT + 3; i++)
            for (j = 0; j < MAP_WIDTH; j++)
                if (grid[i][j] == 1)
                {
                    attron(COLOR_PAIR(BG_LIGHTGREEN));
                    mvaddch(i, j, ' ');
                }
                else
                {
                    attron(COLOR_PAIR(BG_DARKGREEN));
                    mvaddch(i, j, ' ');
                }

        //message background
        for (i = 0; i < 7; i++)
        {
            for (j = 0; j < 40; j++)
            {
                attron(COLOR_PAIR(BG_WHITE));
                mvaddch(i + (MAP_HEIGHT + 3) / 2 - 4, j + MAP_WIDTH / 2 - 40 / 2, ' ');
            }
        }

        //message
        for (i = 0; i < 5; i++)
        {
            for (j = 0; j < 38; j++)
            {
                attron(COLOR_PAIR(BG_BLACK));
                mvprintw(i + MAP_HEIGHT / 2 - 1, j + MAP_WIDTH / 2 - 38 / 2, you_win);
            }
        }

        attron(COLOR_PAIR(BG_RED));
        mvprintw(20, MAP_WIDTH / 2 - 11, "Press any key to quit");

        refresh();
        if (getch() != ERR)
            break;
    }
}

// void draw_movement_cost(Dungeon *d)
// {
//     printf("\nMovement Cost Map\n");

//     uint8_t i, j;
//     for (i = 0; i < MAP_HEIGHT; i++)
//     {
//         for (j = 0; j < MAP_WIDTH; j++)
//             if (d->hardness[i][j] < 255)
//                 printf("%d", (d->hardness[i][j] != 0) ? costxy(j, i) : 1);
//             else
//                 putchar('X');

//         printf("\n");
//     }
//     return;
// }

// void draw_distance_map(Dungeon *d, uint8_t mode)
// {
//     if (mode)
//         printf("\nTunneling Distance Map\n");
//     else
//         printf("\nNon Tunneling Map\n");

//     uint8_t i, j;
//     for (i = 0; i < MAP_HEIGHT; i++)
//     {
//         for (j = 0; j < MAP_WIDTH; j++)
//         {
//             if (d->player.y != i || d->player.x != j)
//             {
//                 int value;

//                 if (mode)
//                     value = d->tunneling_map[i][j];
//                 else
//                     value = d->non_tunneling_map[i][j];

//                 if (value > 0)
//                     printf("%1d", value % 10);
//                 else if (value || mode)
//                     putchar('X');
//                 else
//                     putchar(' ');
//             }
//             else
//             {
//                 printf("@");
//             }
//         }
//         printf("\n");
//     }
//     return;
// }