#ifndef DUNGEON_H
#define DUNGEON_H

// #include <stdint.h>
// #include <position.h>
// #include <room.h>
// #include <character.h>
// #include <hud.h>

#define MAP_WIDTH 80
#define MAP_HEIGHT 21
#define VISUAL_RANGE 15
#define RADIUS 3

#define to_string(s) #s
#define mapxy(x, y) (d->map[y][x])
#define hardnessxy(x, y) (d->hardness[y][x])
#define costxy(x, y) (d->hardness[y][x] / 85 + 1)
#define characterxy(x, y) (d->characters[y][x])

/**
 * Enum of character values for storing in map array
 * */
typedef enum terrain
{
    c_nothin = 0,
    c_rock = ' ',
    c_room = '.',
    c_corridor = '#',
    c_u_stairs = '<',
    c_d_stairs = '>',
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
 * Character struct
 * */

/**
 * Heads up display struct
 * */

class Dungeon
{
public:
    //seed, hardness, and terrain
    uint32_t seed;
    terrain_t map[MAP_HEIGHT][MAP_WIDTH];
    uint8_t hardness[MAP_HEIGHT][MAP_WIDTH];
    terrain_t explored[MAP_HEIGHT][MAP_WIDTH];

    //rooms
    uint16_t num_rooms;
    Room *rooms;

    //up stairs
    uint16_t num_upstairs;
    Position *upstairs;

    //down stairs
    uint16_t num_downstairs;
    Position *downstairs;

    //player and npc
    Position player;
    uint16_t total_deaths;
    uint16_t npc_count;
    Character *characters[MAP_HEIGHT][MAP_WIDTH];
    int32_t non_tunneling_map[MAP_HEIGHT][MAP_WIDTH];
    int32_t tunneling_map[MAP_HEIGHT][MAP_WIDTH];

    //heads up display
    Hud hud;

    //game mode
    game_mode_t mode;
};

#endif