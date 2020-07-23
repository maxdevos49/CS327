#include <climits>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <ncurses.h>

#include "headers/heap.h"
#include "headers/dungeon.h"
#include "headers/path_finding.h"
#include "headers/generator.h"
#include "headers/render.h"
#include "headers/npc.h"

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

            if (d->hardness[i][j] < 255 && mode) //fill heap if not hard wall
                path[i][j].hn = heap_insert(&h, &path[i][j]);
            else if (d->hardness[i][j] == 0) //fill heap if a room or corridor
                path[i][j].hn = heap_insert(&h, &path[i][j]);
            else
                path[i][j].hn = NULL;
        }
    }

    while ((p = (path_finding_t *)heap_remove_min(&h)))
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

void teleport(int8_t *x_cur, int8_t *y_cur, dungeon_t *d)
{
    full_map(d);
    mvaddch(d->player.y + *y_cur + 1, d->player.x + *x_cur, '+');

    while (1)
    {
        switch (getch())
        {
        case 104: //left
        case 52:
            *x_cur -= 1;
            break;
        case 108: //right
        case 54:
            *x_cur += 1;
            break;
        case 107: //up
        case 56:
            *y_cur -= 1;
            break;
        case 106: //down
        case 50:
            *y_cur += 1;
            break;
        case 121: //upper left
        case 55:
            *x_cur -= 1;
            *y_cur -= 1;
            break;
        case 117: //upper right
        case 57:
            *y_cur -= 1;
            *x_cur += 1;
            break;
        case 98: //lower left
        case 49:
            *y_cur += 1;
            *x_cur -= 1;
            break;
        case 110: //lower right
        case 51:
            *x_cur += 1;
            *y_cur += 1;
            break;
        case 116: //t
            draw_map(d);
            return;
            break;
        case 114: //r
            draw_map(d);
            *x_cur = (rand() % MAP_WIDTH) - 3 - d->player.x + 1;
            *y_cur = (rand() % MAP_HEIGHT) - 3 - d->player.y + 1;
            return;
            break;
        default:
            break;
        }

        //bound check x
        if (d->player.x + *x_cur > MAP_WIDTH - 2)
        {
            *x_cur -= 1;
        }
        else if (d->player.x + *x_cur < 1)
        {
            *x_cur += 1;
        }

        //bound check y
        if (d->player.y + *y_cur > MAP_HEIGHT - 2)
        {
            *y_cur -= 1;
        }
        else if (d->player.y + *y_cur < 1)
        {
            *y_cur += 1;
        }

        full_map(d);
        mvaddch(d->player.y + *y_cur + 1, d->player.x + *x_cur, '+');
    }

    return;
}

int cmp_turn(const void *key, const void *with) //wrong
{
    return ((turn_t *)key)->turn - ((turn_t *)with)->turn;
}

void init_heap(heap_t *h, void *t, dungeon_t *d)
{
    uint32_t i, j;
    turn_t *turn = (turn_t *)t;

    heap_init(h, cmp_turn, NULL);
    int id = 0;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            int index = i * MAP_WIDTH + j; //index
            if (d->characters[i][j] != NULL)
            {
                turn[index].pos.y = i;
                turn[index].pos.x = j;
                turn[index].turn = 0;
                turn[index].id = id;
                turn[index].hn = heap_insert(h, &turn[index]);
                id++;
            }
        }
    }

    return;
}

int game_loop(dungeon_t *d)
{
    d->mode = m_play;
    turn_t turn[MAP_HEIGHT][MAP_WIDTH], *p;
    heap_t h;

    init_heap(&h, &turn, d);

    while ((p = (turn_t *)heap_remove_min(&h)))
    {
        p->hn = NULL;

        if (d->characters[p->pos.y][p->pos.x] != NULL)
        {
            if (d->player.x == p->pos.x && d->player.y == p->pos.y)
            {
                draw_map(d);

                if (d->characters[p->pos.y][p->pos.x]->type != 1)
                {
                    d->mode = m_killed;
                    break;
                }

                while (1)
                    if (!move_player(p, d))
                        break;

                //process any player commands/statuses
                if (d->mode == m_gen_floor)
                {
                    change_floor(d, *d->characters[p->pos.y][p->pos.x]);
                    heap_delete(&h);
                    init_heap(&h, &turn, d);
                    draw_map(d);
                    continue;
                }

                if (d->mode != m_play)
                    break;
            }
            else
            {
                //for monsters
                apply_behavior(p, d);
            }

            generate_character_event(&h, &turn[p->pos.y][p->pos.x], p, d);
        }
    }

    heap_delete(&h);
    return 0;
}

int move_player(turn_t *p, dungeon_t *d)
{
    //movement offsets
    int8_t x_offset = 0;
    int8_t y_offset = 0;

    int key = getch(); //waits here
    switch (key)
    {
    case 104: //left
    case 52:
        x_offset = -1;
        break;
    case 108: //right
    case 54:
        x_offset = 1;
        break;
    case 107: //up
    case 56:
        y_offset = -1;
        break;
    case 106: //down
    case 50:
        y_offset = 1;
        break;
    case 121: //upper left
    case 55:
        x_offset = -1;
        y_offset = -1;
        break;
    case 117: //upper right
    case 57:
        x_offset = 1;
        y_offset = -1;
        break;
    case 98: //lower left
    case 49:
        x_offset = -1;
        y_offset = 1;
        break;
    case 110: //lower right
    case 51:
        x_offset = 1;
        y_offset = 1;
        break;
    case 102:
        d->hud.status = (char *)"Peeking is cheating..";
        full_map(d);
        getch();
        draw_map(d);
        return 0;
        break;
    case 116: //teleport
        d->hud.status = (char *)"Teleporting...";
        teleport(&x_offset, &y_offset, d);

        if (x_offset != 0 || y_offset != 0)
        {
            move_character(x_offset, y_offset, p, d);
            distance_maps(d);
        }
        return 0;
        break;
    case 53: //rest
    case 32:
    case 46:
        d->hud.status = (char *)"Resting..";
        return 0;
        break;
    case 81: //quit
        d->mode = m_quit;
        return 0;
        break;
    case 109: //monster list
        monster_list_control(d);
        return 1;
        break;
    case 60: //up
        if (d->map[p->pos.y][p->pos.x] == c_u_stairs)
        {
            d->mode = m_gen_floor;
            d->hud.status = (char *)"You traveled up a floor";
            return 0;
        }
        d->hud.status = (char *)"You can not go up there.";
        update_hud(d);
        return 1;
        break;
    case 62: //down
        if (d->map[p->pos.y][p->pos.x] == c_d_stairs)
        {
            d->mode = m_gen_floor;
            d->hud.status = (char *)"You traveled down a floor";
            return 0;
        }
        d->hud.status = (char *)"You can not go down there.";
        update_hud(d);
        return 1;
        break;
    default: //any other key
        d->hud.key = key;
        update_hud(d);
        return 1;
        break;
    }

    if (d->hardness[d->player.y + y_offset][d->player.x + x_offset] != 0)
    {
        d->hud.status = (char *)"Rock is in the way!";
        update_hud(d);
    }
    else
    {
        d->hud.status = (char *)"";
        if (!process_move(x_offset, y_offset, p, d))
            return 0;
    }

    return 1;
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

    d->characters[p->pos.y][p->pos.x]->turns++; //add a turn
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
        if (d->hardness[p->pos.y + y_offset][p->pos.x + x_offset] != 255)
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
            return -1; //cant move because hard wall
        }
    }
    return 0;
}

void move_character(int x_offset, int y_offset, turn_t *p, dungeon_t *d)
{
    if (d->characters[p->pos.y + y_offset][p->pos.x + x_offset] != NULL) //check for other player
    {
        if (d->characters[p->pos.y][p->pos.x]->type == 1)
        {
            d->hud.status = (char *)"You slay a beast!";
            update_hud(d);
        }

        free(d->characters[p->pos.y + y_offset][p->pos.x + x_offset]); //kill
        d->characters[p->pos.y + y_offset][p->pos.x + x_offset] = NULL;
        d->characters[p->pos.y][p->pos.x]->kills++;
        d->total_deaths++;
    }
    d->characters[p->pos.y + y_offset][p->pos.x + x_offset] = d->characters[p->pos.y][p->pos.x];
    d->characters[p->pos.y][p->pos.x] = NULL;

    d->characters[p->pos.y + y_offset][p->pos.x + x_offset]->pos.y = p->pos.y + y_offset;
    d->characters[p->pos.y + y_offset][p->pos.x + x_offset]->pos.x = p->pos.x + x_offset;

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
    int8_t x_offset = 0;
    int8_t y_offset = 0;
    pos_t next;
    next.x = 0;
    next.y = 0;

    //
    npc_next_pos(d, d->characters[p->pos.y][p->pos.x], &x_offset, &y_offset);

    //stand in ai that works
    // if (d->characters[p->pos.y][p->pos.x]->behavior <= 7) //non_tunnel
    //     lowest_xy(&x_offset, &y_offset, p->pos.x, p->pos.y, d, 1);
    // else //tunnel
    //     lowest_xy(&x_offset, &y_offset, p->pos.x, p->pos.y, d, 0);

    //do move
    process_move(x_offset, y_offset, p, d);

    return 0;
}
