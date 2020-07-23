#ifndef path_finding_h
#define path_finding_h

#include "heap.h"

struct turn;
typedef struct turn turn_t;

/**
 * Calculates the distance map for both tunneling and non tunneling arrays
 * */
void distance_maps(Dungeon *d);

/**
 * Calculates the distance for tunneling
 * */
void tunneling_distance(Dungeon *d);

/**
 * calculates the distance for non tunneling
 * */
void non_tunneling_distance(Dungeon *d);

/**
 * Runs through all turns for a single frame
 * */
int game_loop(Dungeon *d);

/**
 * 
 * Moves a character to an x,y location.
 * */
void move_character(int x_offset, int y_offset, turn_t *p, Dungeon *d);

/**
 * Checks if a character can legally move to a desired location. Then moves the character
 * */
int process_move(int x_offset, int y_offset, turn_t *p, Dungeon *d);

/**
 * Generates a new character event
 * */
void generate_character_event(heap_t *h, turn_t *turn, turn_t *p, Dungeon *d);

/**
 * Applys the behaviors to monsters
 * */
int apply_behavior(turn_t *p, Dungeon *d);

/**
 * Moves the player
 * */
int move_player(turn_t *p, Dungeon *d);

#endif