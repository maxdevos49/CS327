#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "headers/heap.h"
#include "headers/dungeon.h"
#include "headers/path_finding.h"
#include "headers/generator.h"
#include "headers/render.h"

typedef void (*func)(void); //behaviors

typedef struct turn
{
    heap_node_t *hn;
    pos_t pos;
    uint32_t turn;
    int id;
} turn_t;

typedef struct path_finding
{
    heap_node_t *hn;
    pos_t pos;
    pos_t from;
    int32_t cost;
} path_finding_t;

static int32_t path_finding_cmp(const void *key, const void *with)
{
    return ((path_finding_t *)key)->cost - ((path_finding_t *)with)->cost;
}

static void dijkstra(dungeon_t *d, uint8_t mode)
{
    static path_finding_t path[MAP_HEIGHT][MAP_WIDTH], *p;
    heap_t h;
    uint32_t i, j;

    heap_init(&h, path_finding_cmp, NULL);

    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            path[i][j].pos.y = i;
            path[i][j].pos.x = j;
            path[i][j].cost = (d->player.x != j || d->player.y != i) ? INT_MAX : 0;

            if (mapxy(j, i) != c_h_wall && mapxy(j, i) != c_v_wall && mode) //fill heap if not hard wall
                path[i][j].hn = heap_insert(&h, &path[i][j]);
            else if (d->hardness[i][j] == 0) //fill heap if a room or corridor
                path[i][j].hn = heap_insert(&h, &path[i][j]);
            else
                path[i][j].hn = NULL;
        }
    }

    while ((p = heap_remove_min(&h)))
    {
        p->hn = NULL;

        int8_t offset[3] = {-1, 0, 1};

        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if ((path[p->pos.y + offset[i]][p->pos.x + offset[j]].hn) && (path[p->pos.y + offset[i]][p->pos.x + offset[j]].cost > p->cost + costxy(p->pos.x, p->pos.y)))
                {
                    path[p->pos.y + offset[i]][p->pos.x + offset[j]].cost = p->cost + costxy(p->pos.x, p->pos.y);
                    path[p->pos.y + offset[i]][p->pos.x + offset[j]].from.y = p->pos.y;
                    path[p->pos.y + offset[i]][p->pos.x + offset[j]].from.x = p->pos.x;
                    heap_decrease_key_no_replace(&h, path[p->pos.y + offset[i]][p->pos.x + offset[j]].hn);
                }
            }
        }

        //add cost
        if (mode)
            d->tunneling_map[p->pos.y][p->pos.x] = p->cost;
        else
            d->non_tunneling_map[p->pos.y][p->pos.x] = (p->cost == INT_MAX) ? p->cost + 1 : p->cost;
    }

    heap_delete(&h);
    return;
}

void distance_maps(dungeon_t *d)
{
    dijkstra(d, 1); //tunneling
    dijkstra(d, 0); //non tunneling
}

void tunneling_distance(dungeon_t *d)
{
    dijkstra(d, 1);
    return;
}

void non_tunneling_distance(dungeon_t *d)
{
    dijkstra(d, 0);
    return;
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

int cmp_turn(const void *key, const void *with) //wrong
{
    return ((turn_t *)key)->turn - ((turn_t *)with)->turn;
}

int game_loop(dungeon_t *d)
{
    turn_t turn[MAP_HEIGHT][MAP_WIDTH], *p;
    uint32_t i, j;
    heap_t h;

    heap_init(&h, cmp_turn, NULL);
    int id = 0;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            if (d->characters[i][j] != NULL)
            {
                turn[i][j].pos.y = i;
                turn[i][j].pos.x = j;
                turn[i][j].turn = 0;
                turn[i][j].id = id;
                turn[i][j].hn = heap_insert(&h, &turn[i][j]);
                id++;
            }
        }
    }

    while ((p = heap_remove_min(&h)))
    {
        p->hn = NULL;

        if (d->characters[p->pos.y][p->pos.x] != NULL)
        {
            if (d->player.x == p->pos.x && d->player.y == p->pos.y)
            {
                draw_map(d);
                usleep(FRAMERATE);

                if (d->characters[p->pos.y][p->pos.x]->type != 1)
                    break;

                int x_offset = (rand() % 3) - 1;
                int y_offset = (rand() % 3) - 1;

                process_move(x_offset, y_offset, p, d);
            }
            else
            {
                apply_behavior(p, d);
            }

            generate_character_event(&h, &turn[p->pos.y][p->pos.x], p, d);

            //debug
            // printf("Monster Id: %2d , X: %2d, Y: %2d , Speed: %2d , Turn: %4d\n", p->id, p->pos.x, p->pos.y, d->characters[p->pos.y][p->pos.x]->speed, p->turn);
        }
    }

    heap_delete(&h);

    return 0;
}

void generate_character_event(heap_t *h, turn_t *turn, turn_t *p, dungeon_t *d)
{
    static uint32_t previous;
    uint16_t index;

    if (d->characters[p->pos.y][p->pos.x]->type)
    { //player specific
        d->player.x = p->pos.x;
        d->player.y = p->pos.y;
    }

    previous = p->turn;
    index = p->pos.y * MAP_WIDTH + p->pos.x;

    *turn = *p;
    turn->turn = previous + 1000 / d->characters[p->pos.y][p->pos.x]->speed;
    turn->hn = heap_insert(h, turn);
}

int process_move(int x_offset, int y_offset, turn_t *p, dungeon_t *d)
{
    if (x_offset || y_offset)
    {
        if (d->hardness[p->pos.y + y_offset][p->pos.x - x_offset] != 255)
        {
            if (d->hardness[p->pos.y + y_offset][p->pos.x + x_offset] != 0)
            {
                if (d->hardness[p->pos.y + y_offset][p->pos.x + x_offset] > 85) //hard rock
                {
                    d->hardness[p->pos.y + y_offset][p->pos.x + x_offset] -= 85;
                    tunneling_distance(d);
                }
                else
                {
                    move_character(x_offset, y_offset, p, d);
                    d->map[p->pos.y][p->pos.x] = c_corridor;
                    d->hardness[p->pos.y][p->pos.x] = 0;
                    distance_maps(d);
                }
            }
            else
            {
                move_character(x_offset, y_offset, p, d);
                if (d->characters[p->pos.y][p->pos.x]->type)
                    distance_maps(d);
            }
        }
        else
        {
            return -1; //cant move because immutable
        }
    }
    return 0;
}

void move_character(int x_offset, int y_offset, turn_t *p, dungeon_t *d)
{
    if (d->characters[p->pos.y + y_offset][p->pos.x + x_offset] != NULL) //check for other player
    {
        free(d->characters[p->pos.y + y_offset][p->pos.x + x_offset]); //kill
        d->characters[p->pos.y + y_offset][p->pos.x + x_offset] = NULL;
    }
    d->characters[p->pos.y + y_offset][p->pos.x + x_offset] = d->characters[p->pos.y][p->pos.x];
    d->characters[p->pos.y][p->pos.x] = NULL;
    p->pos.x += x_offset;
    p->pos.y += y_offset;
    return;
}

void lowest_xy(int *x, int *y, uint8_t pos_x, uint8_t pos_y, dungeon_t *d, uint8_t mode)
{
    uint8_t i, j;
    uint16_t low_hard = UINT16_MAX;
    int offset[] = {-1, 0, 1};

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (i == 1 && j == 1)
                continue;

            if (mode)
            {
                if (low_hard > d->non_tunneling_map[pos_y + offset[i]][pos_x + offset[j]] && d->hardness[pos_y + offset[i]][pos_x + offset[j]] == 0)
                {
                    *x = offset[j];
                    *y = offset[i];
                    low_hard = d->non_tunneling_map[pos_y + offset[i]][pos_x + offset[j]];
                }
            }
            else
            {
                if (low_hard > d->tunneling_map[pos_y + offset[i]][pos_x + offset[j]])
                {
                    *x = offset[j];
                    *y = offset[i];
                    low_hard = d->tunneling_map[pos_y + offset[i]][pos_x + offset[j]];
                }
            }
        }
    }
    return;
}

int apply_behavior(turn_t *p, dungeon_t *d)
{
    int x_offset = 0;
    int y_offset = 0;

    if (d->characters[p->pos.y][p->pos.x]->behavior <= 7) //non_tunnel
        lowest_xy(&x_offset, &y_offset, p->pos.x, p->pos.y, d, 1);
    else //tunnel
        lowest_xy(&x_offset, &y_offset, p->pos.x, p->pos.y, d, 0);

    //do move
    process_move(x_offset, y_offset, p, d);

    return 0;
}
