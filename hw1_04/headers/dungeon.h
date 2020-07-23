#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdint.h>

#define MAP_WIDTH 80
#define MAP_HEIGHT 21
#define COLOR 1
#define FRAMERATE 300000

#define NPC_SMART   0x00000001
#define NPC_TELE    0x00000002
#define NPC_TUNNEL  0x00000004
#define NPC_ERRATIC 0x00000008

#define to_string(s) #s
#define mapxy(x, y) (d->map[y][x])
#define hardnessxy(x, y) (d->hardness[y][x])
#define costxy(x, y) (d->hardness[y][x] / 85 + 1)
#define characterxy(x, y) (d->characters[y][x])

#define has_characteristic(character, bit) ((character)->npc->characteristics & NPC_##bit)

/**
 * Enum of character values for storing in map array
 * */
typedef enum terrain
{
    c_rock,     //
    c_h_wall,   //-
    c_v_wall,   //|
    c_room,     //.
    c_corridor, //#
    c_u_stairs, //<
    c_d_stairs, //>
} terrain_t;

/**
 * Enum of different npcs
 * */
typedef enum npcs
{
    n_player,    //@
    n_human,     //p
    n_non_human, //h
    n_giant,     //P
    n_dragon,    //D
    n_smuag,     //D
    n_ruth,      //D
    n_falcor,    //D
    n_saphira,   //D
    n_norbert,   //D
    n_rand,      //p

} npc_t;

/**
 * Position struct
 * */
typedef struct pos
{
    uint8_t x;
    uint8_t y;
} pos_t;

/**
 * Dimension struct
 * */
typedef struct dim
{
    uint8_t h;
    uint8_t w;
} dim_t;

/**
 * Room Structure
 * */
typedef struct room
{
    uint8_t connected;
    uint8_t occupied;
    pos_t pos;
    dim_t dim;
} room_t;

typedef struct character
{
    uint8_t type;     //monster or player
    uint8_t health;   //100
    uint8_t speed;    //between 5 and 20
    uint8_t behavior; //16 kinds
} character_t;


typedef struct dungeon
{
    //seed, hardness, and terrain
    uint32_t seed;
    terrain_t map[MAP_HEIGHT][MAP_WIDTH];
    uint8_t hardness[MAP_HEIGHT][MAP_WIDTH];

    //rooms
    uint16_t num_rooms;
    room_t *rooms;

    //up stairs
    uint16_t num_upstairs;
    pos_t *upstairs;

    //down stairs
    uint16_t num_downstairs;
    pos_t *downstairs;

    //player and npc
    pos_t player;
    uint16_t npc_count;
    character_t *characters[MAP_HEIGHT][MAP_WIDTH];
    int32_t non_tunneling_map[MAP_HEIGHT][MAP_WIDTH];
    int32_t tunneling_map[MAP_HEIGHT][MAP_WIDTH];

} dungeon_t;

#endif