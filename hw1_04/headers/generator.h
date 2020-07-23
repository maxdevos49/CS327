#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include <stdint.h>

#define MAX_ROOM_ATTEMPTS 1000
#define MIN_ROOMS 6
#define MAX_ROOMS 10
#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 3
#define MAX_ROOM_SIZE 10

/**
 * Generates a dungeon
 * */
int generate(dungeon_t *d);

/**
 * Inits the terrain map and hardness
 * */
void fill_map(dungeon_t *d);

/**
 * Free's the arrays that are part of the dungeon_t struct
 * */
void free_dungeon(dungeon_t *d);

/**
 * Creates a character at an x,y location either as a monster type 0 or player as type 1
 * */
void create_character(dungeon_t *d, uint8_t x, uint8_t y, uint8_t type);

#endif