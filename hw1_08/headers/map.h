#ifndef MAP_H
#define MAP_H

#include <cstdint>
#include <room.h>

class Map
{
private:
    static const uint8_t MIN_ROOM_HEIGHT = 3;
    static const uint8_t MIN_ROOM_WIDTH = 3;
    static const uint8_t MAX_ROOM_SIZE = 10;
    static const uint8_t MIN_ROOMS = 6;
    static const uint8_t MAX_ROOM_ATTEMPTS = 100;

public:
    Map();

    int generate(int given_seed);
    int change_floor(int given_seed);

    //Constants
    static const uint8_t MAP_HEIGHT = 21;
    static const uint8_t MAP_WIDTH = 80;

    int seed;
    uint8_t map[21][80];
    uint8_t hardness[21][80];
    uint8_t explored[21][80];

    uint16_t room_count;
    Room rooms[6]; //use vector later for resizability??

private:
    void init_map();
    void smooth_map();
    void add_rooms();
    void add_cooridors();
    void add_spawn();
    void populate_monsters();
};

// /**
//  * Generates a dungeon
//  * */
// int generate(Dungeon *d);

// /**
//  * Inits the terrain map and hardness
//  * */
// void fill_map(Dungeon *d);

// /**
//  * Free's the arrays that are part of the Dungeon struct
//  * */
// void free_dungeon(Dungeon *d);

// /**
//  * Creates a character at an x,y location either as a monster type 0 or player as type 1
//  * */
// void create_character(Dungeon *d, uint8_t x, uint8_t y, uint8_t type);

// /**
//  * Adds monsters to the screen
//  * */
// void add_monsters(Dungeon *d);

// /**
//  * Changes the floor of the dungeon
//  * */
// int change_floor(Dungeon *d, Character player);

#endif