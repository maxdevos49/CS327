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
void draw_map(Dungeon *d);

/**
 * 
 * */
void full_map(Dungeon *d);

/**
 * Draws a graphical representation of the hardness using hexidecimal
 * */
void draw_hardness(Dungeon *d);

/**
 * Draws the movement cost
 * */
void draw_movement_cost(Dungeon *d);

/**
 * Draws the generated distance maps
 * */
void draw_distance_map(Dungeon *d, uint8_t mode);

/**
 * Draws the Hud
 * */
void update_hud(Dungeon *d);

/**
 * Shows the quit screen
 * */
void show_quit(Dungeon *d);

/**
 * Shows the lose screen
 * */
void show_death(Dungeon *d);

/**
 * Renders the monster list to the screen
 * */
void draw_monster_list(Dungeon *d, int8_t direction);

/**
 * controls the monster list
 * */
void monster_list_control(Dungeon *d);

#endif