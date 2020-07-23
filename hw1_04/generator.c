#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "headers/dungeon.h"
#include "headers/dungeon.h"
#include "headers/generator.h"

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
                d->map[i][j] = c_h_wall;
                d->hardness[i][j] = 255;
            }
            else if (j == 0 || j == MAP_WIDTH - 1) //x borders
            {
                d->map[i][j] = c_v_wall;
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
        }
    }
}

//rename gaussian maybe
int apply_square_matrix(int matrix_size, int matrix[matrix_size][matrix_size], uint8_t x, uint8_t y, uint8_t map[MAP_HEIGHT][MAP_WIDTH])
{
    uint32_t sum = 0;

    uint8_t i, j;
    for (i = 0; i < matrix_size; i++)
        for (j = 0; j < matrix_size; j++)
            sum += matrix[i][j] * map[y + (i - 1)][x + (j - 1)];

    return (sum / (matrix_size * matrix_size));
}

void smooth_hardness(dungeon_t *d) //needs redone due to int overflow cause hardness 0 in bad loactions
{
    uint8_t i, j;
    //smooth hardness with gausian filter
    int matrix[3][3] = {
        {0, 2, 0},
        {2, 4, 2},
        {0, 2, 0}};

    uint8_t reference[MAP_HEIGHT][MAP_WIDTH];
    for (i = 1; i < MAP_HEIGHT - 1; i++)
        for (j = 1; j < MAP_WIDTH - 1; j++)
            reference[i][j] = d->hardness[i][j];

    for (i = 1; i < MAP_HEIGHT - 1; i++)
        for (j = 1; j < MAP_WIDTH - 1; j++)
            d->hardness[i][j] = apply_square_matrix(3, matrix, j, i, reference);

    return;
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

    d->rooms = malloc(sizeof(room_t) * MAX_ROOMS);

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
    d->upstairs = malloc(sizeof(pos_t));
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
            d->upstairs = malloc(sizeof(pos_t));
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
}



int generate(dungeon_t *d)
{
    if (!d->seed)
        d->seed = time(NULL);

    srand(d->seed);
    printf("Seed: %d\n", d->seed);

    fill_map(d);
    smooth_hardness(d);// still broken
    place_rooms(d);
    place_corridors(d);
    place_stairs(d);
    place_player(d);

    return 0;
}

void create_character(dungeon_t *d, uint8_t x, uint8_t y, uint8_t type)
{
    d->characters[y][x] = malloc(sizeof(character_t));
    d->characters[y][x]->health = 100;
    d->characters[y][x]->type = type;

    if (type)
    {
        d->characters[y][x]->speed = 10;
        d->characters[y][x]->behavior = 0;
    }
    else
    {
        d->characters[y][x]->speed = (rand() % 16) + 5;
        d->characters[y][x]->behavior = (rand() % 16);
    }
}