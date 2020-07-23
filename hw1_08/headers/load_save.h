#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

/**
 * Enum for specifying different actions
 * */
typedef enum action
{
    other,
    load,
    save,
    load_save,
} action_t;

/**
 * Loads a dungeon from a file
 * */
int load_dungeon(Dungeon *d);

/**
 * Saves a dungeon to a file
 * */
int save_dungeon(Dungeon *d);

#endif