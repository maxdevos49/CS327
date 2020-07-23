/**
 * Dungeon Generator
 * @author Maxwell DeVos
 * */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "headers/dungeon_generator.h"
#include "headers/color.h"
int apply_square_matrix(int matrix_size, int matrix[matrix_size][matrix_size], int x, int y, int map[MAP_HEIGHT][MAP_WIDTH]);

int main(int argc, char const *argv[])
{
    int seed;
    int room_count;

    cell map[MAP_HEIGHT][MAP_WIDTH];
    room rooms[MIN_ROOMS];

    seed = time(NULL);

    srand(seed);

    fill_map(map);

    //smooth hardness with gausian filter
    int matrix[3][3] = {
        {0, 2, 0},
        {2, 4, 2},
        {0, 2, 0}};

    int reference[MAP_HEIGHT][MAP_WIDTH];

    int i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            reference[i][j] = map[i][j].h;
        }
    }

    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            map[i][j].h = apply_square_matrix(3, matrix, j, i, reference);
        }
    }

    room_count = place_rooms(rooms, map);
    place_corridors(rooms, room_count, map);
    place_stairs(map);
    place_player(map);

    printf("Seed: %d\n", seed);
    draw_map(map);

    // return 0;
}

void fill_map(void *map_array)
{
    int i, j;
    cell *map = (cell *)map_array;

    for (i = 0; i < MAP_HEIGHT; i++) //height
    {
        for (j = 0; j < MAP_WIDTH; j++) //width
        {
            int index = i * MAP_WIDTH + j; //index

            if (i == 0 || i == MAP_HEIGHT - 1) //y borders
            {
                map[index].c = c_h_wall;
                map[index].h = 255;
            }
            else if (j == 0 || j == MAP_WIDTH - 1) //x borders
            {
                map[index].c = c_v_wall;
                map[index].h = 255;
            }
            else
            {
                map[index].c = c_rock;
                map[index].h = (rand() % 254) + 1;
            }
        }
    }
    return;
}

int place_rooms(room *r, void *map_array)
{
    cell *map = (cell *)map_array;
    int room_count = 0;
    int attempts = 0;

    while (room_count < MIN_ROOMS)
    {
        if (add_room(r, room_count, map))
        {
            if (attempts >= MAX_ROOM_ATTEMPTS)
            {
                break;
            }
            attempts++;
            continue;
        }
        attempts = 0;
        room_count++;
    }

    return room_count;
}

int add_room(room *r, int index, void *map_array)
{
    //increment reference
    r += index;

    //generate random position and size
    r->x = rand() % MAP_WIDTH;
    r->y = rand() % MAP_HEIGHT;
    r->width = (rand() % MAX_ROOM_SIZE) + MIN_ROOM_WIDTH;
    r->height = (rand() % MAX_ROOM_SIZE) + MIN_ROOM_HEIGHT;

    if (r->x < 2 || r->x > MAP_WIDTH - (r->width + 2)) //check map bounds horizontaly
    {
        return 1;
    }
    else if (r->y < 2 || r->y > MAP_HEIGHT - (r->height + 2)) //check map bounds vertically
    {
        return 1;
    }

    int i;
    int j;
    cell *map = (cell *)map_array;

    //check if space is clear
    for (i = r->y - 1; i < r->height + r->y + 1; i++)
    {
        for (j = r->x - 1; j < r->width + r->x + 1; j++)
        {
            int position = i * MAP_WIDTH + j;

            if (map[position].c != 0)
            {
                return 1; //found a taken space
            }
        }
    }

    //add the room
    for (i = r->y; i < r->height + r->y; i++)
    {
        for (j = r->x; j < r->width + r->x; j++)
        {
            int position = i * MAP_WIDTH + j;
            map[position].c = 3;
            map[position].h = 0;
        }
    }

    return 0;
}

void place_corridors(room *r, int room_count, void *map_array)
{

    int i;
    for (i = 0; i < room_count - 1; i++)
    {
        int x_cur, y_cur, tx, ty;
        cell *map = (cell *)map_array;

        //start location
        x_cur = r->x + (rand() % r->width);
        y_cur = r->y + (rand() % r->height);

        r++; //increment pointer for target room

        //target location
        tx = r->x + (rand() % r->width);
        ty = r->y + (rand() % r->height);

        while ((x_cur != tx) || (y_cur != ty))
        {
            int horizontal_movement = (x_cur < tx) ? 1 : -1;
            int vertical_movement = (y_cur < ty) ? 1 : -1;

            int pot_x = x_cur + horizontal_movement;
            int pot_y = y_cur + vertical_movement;

            float distance_x = sqrt(pow((tx - pot_x), 2) + pow(ty - y_cur, 2));
            float distance_y = sqrt(pow((tx - x_cur), 2) + pow(ty - pot_y, 2));
            float previous_d = sqrt(pow((tx - x_cur), 2) + pow(ty - y_cur, 2));

            int index_x = y_cur * MAP_WIDTH + pot_x;
            int index_y = pot_y * MAP_WIDTH + x_cur;

            if (distance_x < previous_d && distance_y < previous_d)
            {
                if (map[index_x].h < map[index_y].h)
                {
                    x_cur += horizontal_movement;
                }
                else
                {
                    y_cur += vertical_movement;
                }
            }
            else
            {
                if (distance_x < distance_y)
                {
                    x_cur += horizontal_movement;
                }
                else
                {
                    y_cur += vertical_movement;
                }
            }

            add_corridor(map, x_cur, y_cur);
            // draw_map(map);
        }
    }
    return;
}

void add_corridor(void *map_array, int x, int y)
{
    cell *map = (cell *)map_array;
    int index = y * MAP_WIDTH + x;

    if (map[index].c == c_rock)
    {
        map[index].c = c_corridor;
        map[index].h = 0;
    }
    return;
}

void place_stairs(void *map_array)
{
    cell *map = (cell *)map_array;
    while (1)
    {
        int x = rand() % (MAP_WIDTH - 1) + 1;
        int y = rand() % (MAP_HEIGHT - 1) + 1;

        if (map[y * MAP_WIDTH + x].h == 0 && map[y * MAP_WIDTH + x].c != c_u_stairs && map[y * MAP_WIDTH + x].c != c_player)
        {
            map[y * MAP_WIDTH + x].c = c_d_stairs;
            break;
        }
    }

    while (1)
    {
        int x = rand() % (MAP_WIDTH - 1) + 1;
        int y = rand() % (MAP_HEIGHT - 1) + 1;

        if (map[y * MAP_WIDTH + x].h == 0 && map[y * MAP_WIDTH + x].c != c_d_stairs && map[y * MAP_WIDTH + x].c != c_player)
        {
            map[y * MAP_WIDTH + x].c = c_u_stairs;
            break;
        }
    }
    return;
}

void place_player(void *map_array)
{
    cell *map = (cell *)map_array;

    while (1)
    {
        int x = rand() % (MAP_WIDTH - 1) + 1;
        int y = rand() % (MAP_HEIGHT - 1) + 1;

        if (map[y * MAP_WIDTH + x].h == 0)
        {
            map[y * MAP_WIDTH + x].c = c_player;
            break;
        }
    }
    return;
}

int apply_square_matrix(int matrix_size, int matrix[matrix_size][matrix_size], int x, int y, int map[MAP_HEIGHT][MAP_WIDTH])
{
    int sum = 0;

    int i, j;

    //loop through matrix
    for (i = 0; i < matrix_size; i++)
    {
        for (j = 0; j < matrix_size; j++)
        {
            //sum up the matrix math
            sum += matrix[i][j] * map[y + (i - 1)][x + (j - 1)];
        }
    }

    return (sum / (matrix_size * matrix_size));
}

void draw_map(void *map_array)
{
    int i;
    int j;
    cell *map = (cell *)map_array;

    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            int index = i * MAP_WIDTH + j;

            switch (map[index].c)
            {
            case c_rock:
                if (map[index].h < 85)
                {
                    printf(BG_GREY);
                }
                else if (map[index].h < 170)
                {
                    printf(BG_DARK_GREY);
                }
                else
                {
                    printf(BG_DARK);
                }
                printf(" ");
                break;
            case c_h_wall:
                printf(BG_BLACK  "-");
                break;
            case c_v_wall:
                printf(BG_BLACK "|");
                break;
            case c_room:
                printf(BG_GROUND ".");
                break;
            case c_corridor:
                printf(BG_GROUND "#");
                break;
            case c_player:
                printf(BG_GROUND C_BLUE "@");
                break;
            case c_u_stairs:
                printf(BG_GROUND C_YELLOW ">");
                break;
            case c_d_stairs:
                printf(BG_GROUND C_YELLOW "<");
                break;
            default:
                printf("*");
                break;
            }
            printf(A_C_R);
        }
        printf("\n");
    }
    return;
}