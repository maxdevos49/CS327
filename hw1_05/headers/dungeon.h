#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdint.h>

#define MAP_WIDTH 80
#define MAP_HEIGHT 21
#define COLOR 1
#define VISUAL_RANGE 15

#define to_string(s) #s
#define mapxy(x, y) (d->map[y][x])
#define hardnessxy(x, y) (d->hardness[y][x])
#define costxy(x, y) (d->hardness[y][x] / 85 + 1)
#define characterxy(x, y) (d->characters[y][x])
// #define hardnesspair(pair) (d->hardness[pair.y][pair.x])

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
 * Game mode enum
 * */
typedef enum mode
{
    m_play,        //normal playing
    m_quit,        //player quit
    m_killed,      //player died
    m_gen_floor,   //generate new floor
    m_monster_list //monster list is active
} game_mode_t;

/**
 * Position struct
 * */
typedef struct pos
{
    int8_t x;
    int8_t y;
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

/**
 * Character struct
 * */
typedef struct character
{
    uint8_t type;     //monster or player
    uint8_t health;   //100
    uint8_t speed;    //between 5 and 20
    uint32_t turns;   //turn count
    uint16_t kills;   //kills
    pos_t pos;        //position
    uint32_t behavior; //16 kinds
    uint32_t have_seen_pc;
    pos_t pc_last_known_pos;
} character_t;

/**
 * Heads up display struct
 * */
typedef struct hud
{
    char *status;
    char *message;
    char *seed;
    int key;
} hud_t;

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
    uint16_t total_deaths;
    uint16_t npc_count;
    character_t *characters[MAP_HEIGHT][MAP_WIDTH];
    int32_t non_tunneling_map[MAP_HEIGHT][MAP_WIDTH];
    int32_t tunneling_map[MAP_HEIGHT][MAP_WIDTH];

    //heads up display
    hud_t hud;

    //game mode
    game_mode_t mode;

} dungeon_t;

#endif