#ifndef render_h
#define render_h

#include "dungeon.h"

/**
 * Inits the terminal
 * */
void init_terminal(void);
/**
 * Draws the map based on the given map array
 * */
void draw_map(dungeon_t *d);

/**
 * Draws a graphical representation of the hardness using hexidecimal
 * */
void draw_hardness(dungeon_t *d);

/**
 * Draws the movement cost
 * */
void draw_movement_cost(dungeon_t *d);

/**
 * Draws the generated distance maps
 * */
void draw_distance_map(dungeon_t *d, uint8_t mode);

/**
 * Draws the Hud
 * */
void update_hud(dungeon_t *d);

/**
 * Shows the quit screen
 * */
void show_quit(dungeon_t *d);

/**
 * Shows the lose screen
 * */
void show_death(dungeon_t *d);

/**
 * Renders the monster list to the screen
 * */
void draw_monster_list(dungeon_t *d, int8_t direction);

/**
 * controls the monster list
 * */
void monster_list_control(dungeon_t *d);

#endif