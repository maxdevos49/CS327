#include <cstdio>
#include <time.h>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include "headers/dungeon.h"
#include "headers/generator.h"
#include "headers/path_finding.h"

int gaussian[5][5] = {
    {1, 4, 7, 4, 1},
    {4, 16, 26, 16, 4},
    {7, 26, 41, 26, 7},
    {4, 16, 26, 16, 4},
    {1, 4, 7, 4, 1}};

void gaussian_filter(dungeon_t *d, void *h);

static int smooth_hardness(dungeon_t *d)
{
    int32_t i, j;
    uint8_t hardness[MAP_HEIGHT][MAP_WIDTH];

    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            hardness[i][j] = d->hardness[i][j];
        }
    }

    gaussian_filter(d, &hardness);

    return 0;
}

void fill_map(dungeon_t *d)
{
    uint8_t i, j;

    d->num_rooms = 0;

    for (i = 0; i < MAP_HEIGHT; i++) //height
    {
        for (j = 0; j < MAP_WIDTH; j++) //width
        {
            if (i == 0 || i == MAP_HEIGHT - 1) //y borders
            {
                d->map[i][j] = c_rock;
                d->hardness[i][j] = 255;
            }
            else if (j == 0 || j == MAP_WIDTH - 1) //x borders
            {
                d->map[i][j] = c_rock;
                d->hardness[i][j] = 255;
            }
            else
            {
                d->map[i][j] = c_rock;
                d->hardness[i][j] = (rand() % 254) + 1;
            }
            d->characters[i][j] = NULL;
            d->non_tunneling_map[i][j] = 0;
            d->tunneling_map[i][j] = 0;
            d->explored[i][j] = c_nothin;
        }
    }
}

void gaussian_filter(dungeon_t *d, void *h)
{
    uint8_t *hardness = (uint8_t *)h;
    int y, x, s, t, p, q;
    for (y = 0; y < MAP_HEIGHT; y++)
    {
        for (x = 0; x < MAP_WIDTH; x++)
        {
            for (s = t = p = 0; p < 5; p++)
            {
                for (q = 0; q < 5; q++)
                {
                    if (y + (p - 2) >= 0 && y + (p - 2) < MAP_HEIGHT &&
                        x + (q - 2) >= 0 && x + (q - 2) < MAP_WIDTH)
                    {
                        s += gaussian[p][q];
                        int index = (y + (p - 2)) * MAP_WIDTH + (x + (q - 2));
                        t += hardness[index] * gaussian[p][q];
                    }
                }
            }
            if (d->hardness[y][x] != 255)
                d->hardness[y][x] = t / s;
        }
    }
}

int add_room(dungeon_t *d)
{

    room_t r;

    //generate random position and size
    r.pos.x = rand() % MAP_WIDTH;
    r.pos.y = rand() % MAP_HEIGHT;
    r.dim.w = (rand() % MAX_ROOM_SIZE) + MIN_ROOM_WIDTH;
    r.dim.h = (rand() % MAX_ROOM_SIZE) + MIN_ROOM_HEIGHT;
    r.occupied = 0;

    if (r.pos.x < 2 || r.pos.x > MAP_WIDTH - (r.dim.w + 2)) //check map bounds horizontaly
    {
        return 1;
    }
    else if (r.pos.y < 2 || r.pos.y > MAP_HEIGHT - (r.dim.h + 2)) //check map bounds vertically
    {
        return 1;
    }

    uint8_t i, j;

    //check if space is clear
    for (i = r.pos.y - 1; i < r.dim.h + r.pos.y + 1; i++)
        for (j = r.pos.x - 1; j < r.dim.w + r.pos.x + 1; j++)
            if (d->map[i][j] != c_rock)
                return 1; //found a taken space

    //add the room
    for (i = r.pos.y; i < r.dim.h + r.pos.y; i++)
        for (j = r.pos.x; j < r.dim.w + r.pos.x; j++)
        {
            d->map[i][j] = c_room;
            d->hardness[i][j] = 0;
        }

    //add room reference
    d->rooms[d->num_rooms] = r;
    return 0;
}

void place_rooms(dungeon_t *d)
{
    int attempts = 0;

    d->rooms = (room_t *)malloc(sizeof(room_t) * MAX_ROOMS);

    while (d->num_rooms < MIN_ROOMS)
    {
        if (add_room(d))
        {
            if (attempts >= MAX_ROOM_ATTEMPTS)
            {
                break;
            }
            attempts++;
            continue;
        }
        attempts = 0;
        d->num_rooms++;
    }
    return;
}

void add_corridor(dungeon_t *d, uint8_t x, uint8_t y)
{

    if (d->map[y][x] == c_rock)
    {
        d->map[y][x] = c_corridor;
        d->hardness[y][x] = 0;
    }
    return;
}

void place_corridors(dungeon_t *d)
{

    int i;
    for (i = 0; i < d->num_rooms - 1; i++)
    {
        uint8_t x_cur, y_cur, tx, ty;

        //start location
        x_cur = d->rooms[i].pos.x + (rand() % d->rooms[i].dim.w);
        y_cur = d->rooms[i].pos.y + (rand() % d->rooms[i].dim.h);

        //target location
        tx = d->rooms[i + 1].pos.x + (rand() % d->rooms[i + 1].dim.w);
        ty = d->rooms[i + 1].pos.y + (rand() % d->rooms[i + 1].dim.h);

        while ((x_cur != tx) || (y_cur != ty))
        {
            int8_t horizontal_movement = (x_cur < tx) ? 1 : -1;
            int8_t vertical_movement = (y_cur < ty) ? 1 : -1;

            uint8_t pot_x = x_cur + horizontal_movement;
            uint8_t pot_y = y_cur + vertical_movement;

            float distance_x = sqrt(pow((tx - pot_x), 2) + pow(ty - y_cur, 2));
            float distance_y = sqrt(pow((tx - x_cur), 2) + pow(ty - pot_y, 2));
            float previous_d = sqrt(pow((tx - x_cur), 2) + pow(ty - y_cur, 2));

            if (distance_x < previous_d && distance_y < previous_d)
            {
                if (d->hardness[y_cur][pot_x] < d->hardness[pot_y][y_cur])
                    x_cur += horizontal_movement;
                else
                    y_cur += vertical_movement;
            }
            else
            {
                if (distance_x < distance_y)
                    x_cur += horizontal_movement;
                else
                    y_cur += vertical_movement;
            }

            add_corridor(d, x_cur, y_cur);
        }
    }
    return;
}

void place_stairs(dungeon_t *d)
{
    // uint8_t x, y;
    uint8_t rx, ry, ri;

    //down stairs
    ri = rand() % (d->num_rooms);
    rx = rand() % d->rooms[ri].dim.w + d->rooms[ri].pos.x;
    ry = rand() % d->rooms[ri].dim.h + d->rooms[ri].pos.y;

    d->num_upstairs = 1;
    d->upstairs = (pos_t *)malloc(sizeof(pos_t));
    d->upstairs[0].x = rx;
    d->upstairs[0].y = ry;

    d->map[ry][rx] = c_d_stairs;

    //up stairs
    while (1)
    {
        ri = rand() % (d->num_rooms);
        rx = rand() % d->rooms[ri].dim.w + d->rooms[ri].pos.x;
        ry = rand() % d->rooms[ri].dim.h + d->rooms[ri].pos.y;

        if (d->map[ry][rx] != c_d_stairs)
        {
            d->num_upstairs = 1;
            d->upstairs = (pos_t *)malloc(sizeof(pos_t));
            d->upstairs[0].x = rx;
            d->upstairs[0].y = ry;

            d->map[ry][rx] = c_u_stairs;
            break;
        }
    }
    return;
}

void place_player(dungeon_t *d)
{
    uint8_t x, y, i;

    i = rand() % (d->num_rooms);

    x = (rand() % d->rooms[i].dim.w) + d->rooms[i].pos.x;
    y = (rand() % d->rooms[i].dim.h) + d->rooms[i].pos.y;
    d->rooms[i].occupied = 1; //for monster spawns

    d->player.x = x;
    d->player.y = y;

    create_character(d, x, y, 1);

    return;
}

void free_dungeon(dungeon_t *d)
{
    //free upstairs array
    free(d->upstairs);
    //free downstairs array
    free(d->downstairs);
    //free rooms array
    free(d->rooms);

    uint8_t i, j;
    //free any monsters
    for (i = 0; i < MAP_HEIGHT; i++)
        for (j = 0; j < MAP_WIDTH; j++)
            if (d->characters[i][j] != NULL)
                free(d->characters[i][j]);

    return;
}

void create_character(dungeon_t *d, uint8_t x, uint8_t y, uint8_t type)
{
    d->characters[y][x] = (character_t *)malloc(sizeof(character_t));
    d->characters[y][x]->health = 100;
    d->characters[y][x]->type = type;
    d->characters[y][x]->kills = 0;
    d->characters[y][x]->turns = 0;
    d->characters[y][x]->pos.x = x;
    d->characters[y][x]->pos.y = y;

    if (type)
    {
        d->characters[y][x]->speed = 10;
        d->characters[y][x]->behavior = 0;
    }
    else
    {
        d->characters[y][x]->speed = (rand() % 16) + 5;
        d->characters[y][x]->behavior = rand() & 0x0000000f;
    }
}

void add_monsters(dungeon_t *d)
{
    uint16_t i;
    for (i = 0; i < d->npc_count; i++)
    {
        uint16_t ri = (rand() % d->num_rooms);

        if (d->rooms[ri].occupied)
            continue;

        uint8_t x = (rand() % d->rooms[ri].dim.w) + d->rooms[ri].pos.x;
        uint8_t y = (rand() % d->rooms[ri].dim.h) + d->rooms[ri].pos.y;

        create_character(d, x, y, 0);
    }

    return;
}

int generate(dungeon_t *d)
{
    if (!d->seed)
        d->seed = time(NULL);

    srand(d->seed);

    fill_map(d);
    smooth_hardness(d);
    place_rooms(d);
    place_corridors(d);
    place_stairs(d);
    place_player(d);
    add_monsters(d);
    distance_maps(d);

    return 0;
}

int change_floor(dungeon_t *d, character_t player)
{
    free_dungeon(d);
    d->seed = time(NULL);
    generate(d);
    d->characters[d->player.y][d->player.x]->health = player.health;
    d->characters[d->player.y][d->player.x]->kills = player.kills;
    d->characters[d->player.y][d->player.x]->turns = player.turns;

    d->mode = m_play;
    return 0;
}
